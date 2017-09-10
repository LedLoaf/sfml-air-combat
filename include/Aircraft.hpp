#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "Command.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity
{
	public:
	enum class Type
	{
	Eagle,
	Raptor,
	Avenger,
	TypeCount
	};

	public:
	explicit Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual void remove();
	virtual bool isMarkedForRemoval() const;
	bool isAllied() const;
	float getSpeed() const;

	void increaseFireRate();
	void increaseSpread();
	void collectMissiles(unsigned int count);

	void fire();
	void launchMissile();

	void playLocalSound(CommandQueue& commands, SoundEffectID effect);

	int getScoreValue();
	bool isScoreCounted() const;

	private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateMovementPattern(sf::Time dt);
	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void createPickup(SceneNode& node, const TextureHolder& textures) const;

	void updateTexts();
	void updateRollAnimation();

	private:
	Type mType;
	sf::Sprite mSprite;
	Animation mExplosion;

	Command mFireCommand;
	Command mMissileCommand;
	Command mDropPickupCommand;

	sf::Time mFireCountdown;
	bool mIsFiring;
	bool mIsLaunchingMissile;
	int mFireRateLevel;
	int mSpreadLevel;
	int mMissileAmmo;

	bool mShowExplosion;
	bool mSpawnedPickup;
	bool mPlayedExplosionSound;

	float mTravelledDistance;
	std::size_t mDirectionIndex;

	TextNode* mHealthDisplay;
	TextNode* mMissileDisplay;

	bool mScoreCounted;
};

#endif // AIRCRAFT_HPP
