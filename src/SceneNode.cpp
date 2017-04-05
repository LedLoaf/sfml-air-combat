#include "SceneNode.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"

#include <cassert>
#include <algorithm>

SceneNode::SceneNode(Category category)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
{
}

void SceneNode::attachChild(std::unique_ptr<SceneNode> child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&] (std::unique_ptr<SceneNode>& p) -> bool { return p.get() == &node; });
	assert(found != mChildren.end());
	std::unique_ptr<SceneNode> result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// This method is meant to be overriden
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (const std::unique_ptr<SceneNode>& child : mChildren)
	{
		child->update(dt, commands);
	}
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const // states holds the information where to render the parent node
{
	states.transform *= getTransform(); // add current node's transform (position, rotation, scale relative to parent node)
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// This method is meant to be overriden
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const std::unique_ptr<SceneNode>& child : mChildren)
	{
		child->draw(target, states);
	}
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity; // not necessary to initialise, but clearer
	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		transform = node->getTransform() * transform;
	}
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

unsigned int SceneNode::getCategory() const
{
	return static_cast<unsigned int>(mDefaultCategory);
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	if (command.category & getCategory())
	{
		command.action(*this, dt);
	}
	for (const auto& child : mChildren)
	{
		child->onCommand(command, dt);
	}
}

// check the whole scene graph against all nodes
void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for (auto& child : sceneGraph.mChildren)
	{
		checkSceneCollision(*child, collisionPairs);
	}
}

// check the whole scene graph against one node
void SceneNode::checkNodeCollision(SceneNode& node, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
	{
		collisionPairs.insert(std::minmax(this, &node));
	}

	for (auto& child : mChildren)
	{
		child->checkNodeCollision(node, collisionPairs);
	}
}

void SceneNode::removeWrecks()
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

