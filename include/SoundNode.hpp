#ifndef SOUNDNODE_HPP
#define SOUNDNODE_HPP

#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"

class SoundPlayer;

class SoundNode : public SceneNode
{
	public:
		explicit SoundNode(SoundPlayer& player);
		void playSound(SoundEffectID sound);
		virtual unsigned int getCategory() const;

	private:
		SoundPlayer& mSounds;
};

#endif // SOUNDNODE_HPP
