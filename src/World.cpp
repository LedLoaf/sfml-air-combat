#include "World.hpp"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "ParticleNode.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <string>

World::World(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mFonts(fonts)
, mSounds(sounds)
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
, mEnemySpawnPoints()
, mActiveEnemies()
, mScore(0)
, mScoreDisplay(nullptr)
, mScorePosition(15.f, mWorldBounds.height - mWorldView.getSize().y + 15.f)
{
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);
}

void World::loadTextures()
{
	mTextures.load(TextureID::Jungle, "media/textures/Jungle.png");
	mTextures.load(TextureID::Entities, "media/textures/Entities.png");
	mTextures.load(TextureID::Explosion, "media/textures/Explosion.png");
	mTextures.load(TextureID::Particle, "media/textures/Particle.png");
	mTextures.load(TextureID::FinishLine, "media/textures/FinishLine.png");
}

void World::buildScene()
{
	// create and attach layer nodes
	std::size_t nLayers = static_cast<std::size_t>(Layer::LayerCount);
	for (std::size_t i = 0; i < nLayers; i++)
	{
		Category category = (i == static_cast<std::size_t>(Layer::LowerAir)) ? Category::Scene : Category::None;

		auto layer = std::make_unique<SceneNode>(category);
		mSceneLayers[i] = layer.get(); // std::unique_ptr::get() returns a raw pointer, does not transfer ownership

		mSceneGraph.attachChild(std::move(layer));
	}

	// prepare tiled background
	sf::Texture& jungleTexture = mTextures.get(TextureID::Jungle);
	jungleTexture.setRepeated(true);

	float viewHeight = mWorldView.getSize().y;
	sf::IntRect jungleTextureRect(mWorldBounds);
	jungleTextureRect.height += static_cast<int>(viewHeight);

	// add background sprite
	auto jungleSprite = std::make_unique<SpriteNode>(jungleTexture, jungleTextureRect);
	jungleSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
	mSceneLayers[static_cast<std::size_t>(Layer::Background)]->attachChild(std::move(jungleSprite));

	// add finish line
	sf::Texture& finishTexture = mTextures.get(TextureID::FinishLine);
	auto finishSprite = std::make_unique<SpriteNode>(finishTexture);
	finishSprite->setPosition(0.f, -76.f);
	mSceneLayers[static_cast<std::size_t>(Layer::Background)]->attachChild(std::move(finishSprite));

	// add score node
	auto score = std::make_unique<TextNode>(mFonts, "SCORE: 0");
	mScoreDisplay = score.get();
	mScoreDisplay->setPosition(mScorePosition);
	mSceneLayers[static_cast<std::size_t>(Layer::Background)]->attachChild(std::move(score));

	// add particle node
	auto smokeNode = std::make_unique<ParticleNode>(Particle::Type::Smoke, mTextures);
	mSceneLayers[static_cast<std::size_t>(Layer::LowerAir)]->attachChild(std::move(smokeNode));

	// add propellant particle node
	auto propellantNode = std::make_unique<ParticleNode>(Particle::Type::Propellant, mTextures);
	mSceneLayers[static_cast<std::size_t>(Layer::LowerAir)]->attachChild(std::move(propellantNode));

	// add sound effect node
	auto soundNode = std::make_unique<SoundNode>(mSounds);
	mSceneGraph.attachChild(std::move(soundNode));

	// add player's aircraft
	auto player = std::make_unique<Aircraft>(Aircraft::Type::Eagle, mTextures, mFonts);
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[static_cast<std::size_t>(Layer::UpperAir)]->attachChild(std::move(player));

	// add enemy aircrafts
	addEnemies();
}

void World::addEnemies()
{
	addEnemy(Aircraft::Type::Raptor,    0.f,  500.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 1000.f);
	addEnemy(Aircraft::Type::Raptor, +100.f, 1150.f);
	addEnemy(Aircraft::Type::Raptor, -100.f, 1150.f);
	addEnemy(Aircraft::Type::Avenger,  70.f, 1500.f);
	addEnemy(Aircraft::Type::Avenger, -70.f, 1500.f);
	addEnemy(Aircraft::Type::Avenger, -70.f, 1710.f);
	addEnemy(Aircraft::Type::Avenger,  70.f, 1700.f);
	addEnemy(Aircraft::Type::Avenger,  30.f, 1850.f);
	addEnemy(Aircraft::Type::Raptor,  300.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor, -300.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 2500.f);
	addEnemy(Aircraft::Type::Avenger,-300.f, 2700.f);
	addEnemy(Aircraft::Type::Avenger,-300.f, 2700.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 3000.f);
	addEnemy(Aircraft::Type::Raptor,  250.f, 3250.f);
	addEnemy(Aircraft::Type::Raptor, -250.f, 3250.f);
	addEnemy(Aircraft::Type::Avenger,   0.f, 3500.f);
	addEnemy(Aircraft::Type::Avenger,   0.f, 3700.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 3800.f);
	addEnemy(Aircraft::Type::Avenger,   0.f, 4000.f);
	addEnemy(Aircraft::Type::Avenger,-200.f, 4200.f);
	addEnemy(Aircraft::Type::Raptor,  200.f, 4200.f);
	addEnemy(Aircraft::Type::Raptor,    0.f, 4400.f);

	// sort by y coordinate, so only back of array needs to be checked each time
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [] (SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
	// coordinates relative to player's spawn position
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::update(sf::Time dt)
{
	// scroll the view, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();
	guideMissiles();

	// forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}

	// adapt velocity (scrolling, diagonal correction)
	adaptPlayerVelocity();

	handleCollisions();
	updateScore();
	mSceneGraph.removeWrecks();
	spawnEnemies();

	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
	mScoreDisplay->move(0.f, mScrollSpeed * dt.asSeconds());

	updateSounds();
}

bool World::hasAlivePlayer() const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::playerHasReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

void World::guideMissiles()
{
	Command enemyCollector;
	enemyCollector.category = static_cast<unsigned int>(Category::EnemyAircraft);
	enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
	{
		if (!enemy.isDestroyed())
		{
			mActiveEnemies.push_back(&enemy);
		}
	});

	Command missileGuider;
	missileGuider.category = static_cast<unsigned int>(Category::AlliedProjectile);
	missileGuider.action = derivedAction<Projectile>([this] (Projectile& missile, sf::Time)
	{
		if (!missile.isGuided())
		{
			return;
		}

		float minDistance = std::numeric_limits<float>::max();
		Aircraft* closestEnemy = nullptr;
		for (Aircraft* enemy : mActiveEnemies)
		{
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance)
			{
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)
		{
			missile.guideTowards(closestEnemy->getWorldPosition());
		}
	});
	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
}

void World::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);

		mSceneLayers[static_cast<std::size_t>(Layer::UpperAir)]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<unsigned int>(Category::Projectile) | static_cast<unsigned int>(Category::EnemyAircraft);
	command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
		{
			e.remove();
		}
	});

	mCommandQueue.push(command);
}

void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f playerAircraftPosition = mPlayerAircraft->getPosition();
	playerAircraftPosition.x = std::max(playerAircraftPosition.x, viewBounds.left + borderDistance);
	playerAircraftPosition.x = std::min(playerAircraftPosition.x, viewBounds.left + viewBounds.width - borderDistance);
	playerAircraftPosition.y = std::max(playerAircraftPosition.y, viewBounds.top + borderDistance);
	playerAircraftPosition.y = std::min(playerAircraftPosition.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(playerAircraftPosition);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f playerAircraftVelocity = mPlayerAircraft->getVelocity();

	if (playerAircraftVelocity.x != 0.f && playerAircraftVelocity.y != 0.f)
	{
		mPlayerAircraft->setVelocity(playerAircraftVelocity / std::sqrt(2.f));
	}

	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

void World::updateSounds()
{
	mSounds.removeStoppedSounds();
}

void World::updateScore()
{
	Command command;
	command.category = static_cast<unsigned int>(Category::EnemyAircraft);
	command.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
	{
		if (!enemy.isScoreCounted() && enemy.isDestroyed())
		{
			mScore += enemy.getScoreValue();
		}
	});

	mCommandQueue.push(command);

	mScoreDisplay->setString("SCORE: " + std::to_string(mScore), false);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// return view bounds extended with a rectangle at the top, where enemies spawn out of view
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::handleCollisions()
{
	std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (auto pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);

			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}

		else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
			player.playLocalSound(mCommandQueue, SoundEffectID::CollectPickup);
		}

		else if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile) || matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			aircraft.damage(projectile.getDamage());
			projectile.destroy();
		}
	}
}

bool matchesCategories(std::pair<SceneNode*, SceneNode*>& colliders, Category category1, Category category2)
{
	unsigned int nodeCategory1 = colliders.first->getCategory();
	unsigned int nodeCategory2 = colliders.second->getCategory();

	if (static_cast<unsigned int>(category1) & nodeCategory1 && static_cast<unsigned int>(category2) & nodeCategory2)
	{
		return true;
	}
	else if (static_cast<unsigned int>(category1) & nodeCategory2 && static_cast<unsigned int>(category2) & nodeCategory1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

