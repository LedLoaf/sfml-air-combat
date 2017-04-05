#include "TitleState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, mText()
, mShowText(true)
, mTextBlinkTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::TitleScreen));

	mText.setFont(context.fonts->get(FontID::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	if (mShowText)
	{
		window.draw(mText);
	}
}

bool TitleState::update(sf::Time dt)
{
	mTextBlinkTime += dt;
	if (mTextBlinkTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextBlinkTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
	}

	return true;
}

