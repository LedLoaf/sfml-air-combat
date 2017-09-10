#ifndef STATE_HPP
#define STATE_HPP

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;
class MusicPlayer;
class SoundPlayer;

class State
{
	public:
		// holder of shared objects such as textures or Player between different states
		struct Context
		{
			Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player,
				MusicPlayer& music, SoundPlayer& sounds);
			sf::RenderWindow* window;
			TextureHolder* textures;
			FontHolder* fonts;
			Player* player;
			MusicPlayer* music;
			SoundPlayer* sounds;
		};

	public:
		State(StateStack& stack, Context context);
		virtual ~State();

		virtual void draw() = 0;
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& event) = 0;

	protected:
		void requestStackPush(StateID stateID);
		void requestStackPop();
		void requestStackClear();

		Context getContext() const;

	private:
		StateStack* mStack;
		Context mContext;
};

#endif // STATE_HPP
