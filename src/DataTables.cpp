#include "DataTables.hpp"
#include "Aircraft.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Particle.hpp"

using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(static_cast<unsigned int>(Aircraft::Type::TypeCount));

	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].hitpoints = 100;
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].speed = 200.f;
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].textureRect = sf::IntRect(0, 0, 48, 64);
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].hasRollAnimation = true;
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].fireInterval = sf::seconds(1);
	data[static_cast<unsigned int>(Aircraft::Type::Eagle)].scoreValue = 0;

	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].hitpoints = 20;
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].speed = 80.f;
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].textureRect = sf::IntRect(144, 0, 84, 64);
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].hasRollAnimation = false;
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].fireInterval = sf::Time::Zero;
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].directions.push_back(Direction( 45.f, 80.f));
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].directions.push_back(Direction(-45.f, 160.f));
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].directions.push_back(Direction( 45.f, 80.f));
	data[static_cast<unsigned int>(Aircraft::Type::Raptor)].scoreValue = 10;

	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].hitpoints = 40;
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].speed = 50.f;
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].hasRollAnimation = false;
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].textureRect = sf::IntRect(228, 0, 60, 59);
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].fireInterval = sf::seconds(2);
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].directions.push_back(Direction( 45.f, 50.f));
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].directions.push_back(Direction(  0.f, 50.f));
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].directions.push_back(Direction(-45.f, 100.f));
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].directions.push_back(Direction(  0.f, 50.f));
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].directions.push_back(Direction( 45.f, 50.f));
	data[static_cast<unsigned int>(Aircraft::Type::Avenger)].scoreValue = 20;

	return data;
}

std::vector<ProjectileData>	initializeProjectileData()
{
	std::vector<ProjectileData> data(static_cast<unsigned int>(Projectile::Type::TypeCount));

	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].damage = 10;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].speed = 300.f;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Projectile::Type::AlliedBullet)].textureRect = sf::IntRect(175, 64, 3, 14);

	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].damage = 10;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].speed = 300.f;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Projectile::Type::EnemyBullet)].textureRect = sf::IntRect(178, 64, 3, 14);

	data[static_cast<unsigned int>(Projectile::Type::Missile)].damage = 200;
	data[static_cast<unsigned int>(Projectile::Type::Missile)].speed = 150.f;
	data[static_cast<unsigned int>(Projectile::Type::Missile)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Projectile::Type::Missile)].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(static_cast<unsigned int>(Pickup::Type::TypeCount));

	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].textureRect = sf::IntRect(0, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::HealthRefill)].action = [] (Aircraft& a) { a.repair(25); };

	data[static_cast<unsigned int>(Pickup::Type::MissileRefill)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Pickup::Type::MissileRefill)].textureRect = sf::IntRect(40, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::MissileRefill)].action = std::bind(&Aircraft::collectMissiles, _1, 3);

	data[static_cast<unsigned int>(Pickup::Type::FireSpread)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Pickup::Type::FireSpread)].textureRect = sf::IntRect(80, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::FireSpread)].action = std::bind(&Aircraft::increaseSpread, _1);

	data[static_cast<unsigned int>(Pickup::Type::FireRate)].texture = TextureID::Entities;
	data[static_cast<unsigned int>(Pickup::Type::FireRate)].textureRect = sf::IntRect(120, 64, 40, 40);
	data[static_cast<unsigned int>(Pickup::Type::FireRate)].action = std::bind(&Aircraft::increaseFireRate, _1);

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(static_cast<unsigned int>(Particle::Type::TypeCount));

	data[static_cast<unsigned int>(Particle::Type::Propellant)].color = sf::Color(255, 255, 50);
	data[static_cast<unsigned int>(Particle::Type::Propellant)].lifetime = sf::seconds(0.6f);

	data[static_cast<unsigned int>(Particle::Type::Smoke)].color = sf::Color(50, 50, 50);
	data[static_cast<unsigned int>(Particle::Type::Smoke)].lifetime = sf::seconds(4.f);

	return data;
}

