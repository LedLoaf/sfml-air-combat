#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
												Command();

	std::function<void(SceneNode&, sf::Time)>	action;
	unsigned int								category;
};

// this template is to allow the action of a command to use a function with a signature such as void(Aircraft&, sf::Time)
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=] (SceneNode& node, sf::Time dt)
	{
		// check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}

#endif // COMMAND_HPP

