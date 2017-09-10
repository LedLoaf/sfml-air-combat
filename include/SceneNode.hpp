/*

The scene graph owns the scene nodes, therefore it is responsible for their lifetime and destruction. Therefore, each node stores all of its child nodes, which get destroyed if the node is destroyed.

SceneNode inherits from:
- sf::Drawable, since a scene node is meant to be rendered; the purely virtual draw() function of sf::Drawable is overriden;
- sf::Transformable, since it needs to store its current position, rotation and scale;
- sf::NonCopyable (privately), since a scene node cannot be copied.

*/

#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Category.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		explicit SceneNode(Category category = Category::None);

		void attachChild(std::unique_ptr<SceneNode> child);
		std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

		void update(sf::Time dt, CommandQueue& commands);

		sf::Transform getWorldTransform() const;
		sf::Vector2f getWorldPosition() const;

		virtual unsigned int getCategory() const;
		void onCommand(const Command& command, sf::Time dt);

		void checkSceneCollision(SceneNode& sceneGraph, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs);
		void checkNodeCollision(SceneNode& node, std::set<std::pair<SceneNode*, SceneNode*>>& collisionPairs);
		void removeWrecks();
		virtual sf::FloatRect getBoundingRect() const;
		virtual bool isMarkedForRemoval() const;
		virtual bool isDestroyed() const;

	private:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		void updateChildren(sf::Time dt, CommandQueue& commands);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		std::vector<std::unique_ptr<SceneNode>>	mChildren;
		SceneNode* mParent;
		Category mDefaultCategory;
};

float distance(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs);

#endif // SCENENODE_HPP
