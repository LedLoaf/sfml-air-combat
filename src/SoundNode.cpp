#include "SoundNode.hpp"
#include "SoundPlayer.hpp"

SoundNode::SoundNode(SoundPlayer& player)
: SceneNode()
, mSounds(player)
{
}

void SoundNode::playSound(SoundEffectID sound)
{
	mSounds.play(sound);
}

unsigned int SoundNode::getCategory() const
{
	return static_cast<unsigned int>(Category::SoundEffect); 
}
