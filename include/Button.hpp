#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "State.hpp"
#include "SoundPlayer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <functional>

namespace GUI
{

class Button : public Component
{
	public:
		enum class Type
		{
			Normal,
			Selected,
			Pressed,
			TypeCount
		};

	public:
		Button(State::Context context);

		void setCallback(std::function<void()> callback);
		void setText(const std::string& text);
		void setToggle(bool flag);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& event);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void changeTexture(Type buttonType);

	private:
		std::function<void()> mCallback;
		sf::Sprite mSprite;
		sf::Text mText;
		bool mIsToggle;
		SoundPlayer& mSounds;
};

}

#endif // BUTTON_HPP
