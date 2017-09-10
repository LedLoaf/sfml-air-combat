/*

The Player class acts as a controller class to handle user input.

*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"

#include <SFML/Window/Event.hpp>

#include <map>

class CommandQueue;

class Player
{
	public:
		enum class Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			Fire,
			LaunchMissile,
			ActionCount
		};

		enum class MissionStatus
		{
			MissionRunning,
			MissionSuccess,
			MissionFailure
		};

	public:
			Player();

		void handleEvent(const sf::Event& event, CommandQueue& commandQueue);
		void handleRealTimeInput(CommandQueue& commandQueue);

		void assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key getAssignedKey(Action action) const;

		void setMissionStatus(MissionStatus status);
		MissionStatus getMissionStatus() const;

	private:
		void initializeKeys();
		void initializeActions();
		static bool isRealTimeAction(Action action);

	private:
		std::map<sf::Keyboard::Key, Action> mKeyBinding;
		std::map<Action, Command> mActionBinding;
		MissionStatus mCurrentMissionStatus;
};

#endif // PLAYER_HPP
