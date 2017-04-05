#include "SoundPlayer.hpp"

#include <algorithm>

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds()
{
	mSoundBuffers.load(SoundEffectID::AlliedGunfire,	"media/sound/AlliedGunfire.wav");
	mSoundBuffers.load(SoundEffectID::EnemyGunfire,		"media/sound/EnemyGunfire.wav");
	mSoundBuffers.load(SoundEffectID::Explosion1,		"media/sound/Explosion1.wav");
	mSoundBuffers.load(SoundEffectID::Explosion2,		"media/sound/Explosion2.wav");
	mSoundBuffers.load(SoundEffectID::LaunchMissile,	"media/sound/LaunchMissile.wav");
	mSoundBuffers.load(SoundEffectID::CollectPickup,	"media/sound/CollectPickup.wav");
	mSoundBuffers.load(SoundEffectID::Button,			"media/sound/Button.wav");
}

void SoundPlayer::play(SoundEffectID effect)
{
	mSounds.push_back(sf::Sound());
	sf::Sound& sound = mSounds.back();
	sound.setBuffer(mSoundBuffers.get(effect));
	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([] (const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}

