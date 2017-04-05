#include "Projectile.hpp"
#include "EmitterNode.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[static_cast<unsigned int>(type)].texture), Table[static_cast<unsigned int>(type)].textureRect)
, mTargetDirection()
{
	centerOrigin(mSprite);

	if (isGuided())
	{
		auto smoke = std::make_unique<EmitterNode>(Particle::Type::Smoke);
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));

		auto propellant = std::make_unique<EmitterNode>(Particle::Type::Propellant);
		propellant->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(propellant));
	}
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return mType == Missile;
}

unsigned int Projectile::getCategory() const
{
	if (mType == EnemyBullet)
	{
		return static_cast<unsigned int>(Category::EnemyProjectile);
	}
	else
	{
		return static_cast<unsigned int>(Category::AlliedProjectile);
	}
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getSpeed() const
{
	return Table[mType].speed;
}

int Projectile::getDamage() const
{
	return Table[mType].damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate = 200.f;

		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

