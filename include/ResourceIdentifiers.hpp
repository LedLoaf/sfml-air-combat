#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

#include <SFML/Audio/SoundBuffer.hpp>

enum class TextureID
{
	Entities,
	Jungle,
	TitleScreen,
	Buttons,
	Explosion,
	Particle,
	FinishLine,
};

enum class FontID
{
	Main,
};

enum class MusicID
{
	MenuTheme,
	MissionTheme,
};

enum class SoundEffectID
{
	AlliedGunfire,
	EnemyGunfire,
	Explosion1,
	Explosion2,
	LaunchMissile,
	CollectPickup,
	Button,
};

// forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
//	class SoundBuffer;
}

// forward declaration of ResourceHolder
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, TextureID>				TextureHolder;
typedef ResourceHolder<sf::Font, FontID>					FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffectID>		SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS_HPP

