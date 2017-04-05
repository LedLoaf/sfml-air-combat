#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>

class SoundPlayer : private sf::NonCopyable
{
	public:
						SoundPlayer();

		void			play(SoundEffectID effect);

		void			removeStoppedSounds();

	private:
		SoundBufferHolder		mSoundBuffers;
		std::list<sf::Sound>	mSounds; // list because a vector might reallocate playing sounds as new ones are added
};

#endif // SOUNDPLAYER_HPP

