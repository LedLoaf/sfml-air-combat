#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application : private sf::NonCopyable
{
	public:
		Application();
		void run();

	private:
		void processInput();
		void update(sf::Time dt);
		void render();

		void registerStates();

	private:
		static const sf::Time TimePerFrame;

		sf::RenderWindow mWindow;
		TextureHolder mTextures;
		FontHolder mFonts;
		Player mPlayer;

		MusicPlayer mMusic;
		SoundPlayer mSounds;

		StateStack mStateStack;
};

#endif // APPLICATION_HPP
