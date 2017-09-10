#ifndef SETTINGSTATE_HPP
#define SETTINGSTATE_HPP

#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>

class SettingsState : public State
{
	public:
		SettingsState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		void updateLabels();
		void addButton(Player::Action action, float y, const std::string& text, Context context);

	private:
		sf::Sprite mBackgroundSprite;
		GUI::Container mGUIContainer;
		std::array<std::shared_ptr<GUI::Button>, static_cast<unsigned int>(Player::Action::ActionCount)> mBindingButtons;
		std::array<std::shared_ptr<GUI::Label>, static_cast<unsigned int>(Player::Action::ActionCount)> mBindingLabels;
};

#endif // SETTINGSTATE_HPP
