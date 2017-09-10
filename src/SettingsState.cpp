#include "SettingsState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::TitleScreen));

	addButton(Player::Action::MoveLeft, 300.f, "Move Left", context);
	addButton(Player::Action::MoveRight, 350.f, "Move Right", context);
	addButton(Player::Action::MoveUp, 400.f, "Move Up", context);
	addButton(Player::Action::MoveDown, 450.f, "Move Down", context);
	addButton(Player::Action::Fire, 500.f, "Fire", context);
	addButton(Player::Action::LaunchMissile, 550.f, "Missile", context);

	updateLabels();
	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(80.f, 620.f);
	backButton->setText("Back");
	backButton->setCallback([this] ()
	{
		requestStackPop();
	});
	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	for (unsigned int i = 0; i < static_cast<unsigned int>(Player::Action::ActionCount); ++i)
	{
		if (mBindingButtons[i]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(i), event.key.code);
				mBindingButtons[i]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding)
	{
		updateLabels();
	}
	else
	{
		mGUIContainer.handleEvent(event);
	}

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (unsigned int i = 0; i < static_cast<unsigned int>(Player::Action::ActionCount); ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButton(Player::Action action, float y, const std::string& text, Context context)
{
	unsigned i = static_cast<unsigned int>(action);
	mBindingButtons[i] = std::make_shared<GUI::Button>(context);
	mBindingButtons[i]->setPosition(80.f, y);
	mBindingButtons[i]->setText(text);
	mBindingButtons[i]->setToggle(true);

	mBindingLabels[i] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[i]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[i]);
	mGUIContainer.pack(mBindingLabels[i]);
}
