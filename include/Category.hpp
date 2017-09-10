#ifndef CATEGORY_HPP
#define CATEGORY_HPP

enum class Category : unsigned int
{
	None = 0,
	Scene = 1 << 0,
	PlayerAircraft = 1 << 1,
	AlliedAircraft = 1 << 2,
	EnemyAircraft = 1 << 3,
	AlliedProjectile = 1 << 4,
	EnemyProjectile = 1 << 5,
	Pickup = 1 << 6,
	ParticleSystem = 1 << 7,
	SoundEffect = 1 << 8,

	Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,
	Projectile = AlliedProjectile | EnemyProjectile,
};

#endif // CATEGORY_HPP
