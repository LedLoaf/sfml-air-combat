#include "GameState.hpp"
#include "MusicPlayer.hpp"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts, *context.sounds)
, mPlayer(*context.player)
{
	mPlayer.setMissionStatus(Player::MissionStatus::MissionRunning);

	context.music->play(MusicID::MissionTheme);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(Player::MissionStatus::MissionFailure);
		requestStackPush(StateID::GameOver);
	}
	else if (mWorld.playerHasReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionStatus::MissionSuccess);
		requestStackPush(StateID::GameOver);
	}

	CommandQueue& commandQueue = mWorld.getCommandQueue();
	mPlayer.handleRealTimeInput(commandQueue);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commandQueue = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commandQueue);

	// if Escape if pressed, trigger the Pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(StateID::Pause);
	}

	return true;
}

