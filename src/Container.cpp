#include "Container.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

Container::Container()
: mChildren()
, mSelectedChild(-1)
{
}

void Container::pack(std::shared_ptr<Component> component)
{
	mChildren.push_back(component);
	if (!hasSelection() && component->isSelectable())
	{
		select(mChildren.size() - 1);
	}
}

bool Container::isSelectable() const
{
	return false;
}

void Container::handleEvent(const sf::Event& event)
{
	// note: this feature (the first if) does not get used here, but would be very useful for composite GUI hierarchy,
	// e.g. when implementing an inpute box or any kind of component that needs to capture input
	if (hasSelection() && mChildren[mSelectedChild]->isActive())
	{
		mChildren[mSelectedChild]->handleEvent(event);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			selectPrevious();
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			selectNext();
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (hasSelection())
			{
				mChildren[mSelectedChild]->activate();
			}
		}
	}
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (auto child : mChildren)
	{
		target.draw(*child, states);
	}
}

bool Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void Container::select(std::size_t index)
{
	if (mChildren[index]->isSelectable())
	{
		if (hasSelection())
		{
			mChildren[mSelectedChild]->deselect();
		}
		mChildren[index]->select();
		mSelectedChild = index;
	}
}

void Container::selectNext()
{
	if (!hasSelection())
	{
		return;
	}

	int next = mSelectedChild;
	do
	{
		next = (next + 1) % mChildren.size();
	} while(!mChildren[next]->isSelectable());

	select(next);
}

void Container::selectPrevious()
{
	if (!hasSelection())
	{
		return;
	}

	int prev = mSelectedChild;
	do
	{
		prev = (prev + mChildren.size() - 1) % mChildren.size();
	} while(!mChildren[prev]->isSelectable());

	select(prev);
}

}

