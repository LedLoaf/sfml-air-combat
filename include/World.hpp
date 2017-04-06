/*

The World class contains:

1. All the data related to rendering:
	- a reference to the render window
	- a texture holder with all the textures needed in the world
	- the scene graph
	- some pointers to access the scene graph's layer nodes

2. Some logical data:
	- the bounding rectangle of the world, storing its dimensions
	- the spawning position of the player
	- the view's scroll speed
	- a pointer to the player

Concerning functionality, the World class contains:
- public functions to update and draw the world
- private functions to load the textures and build the scene

*/

#ifndef WORLD_HPP
#define WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include "SoundPlayer.hpp"
#include "TextNode.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit			World(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds);

		void				update(sf::Time dt);
		void				draw();

		CommandQueue&		getCommandQueue();

		bool				hasAlivePlayer() const;
		bool				playerHasReachedEnd() const;

	private:
		void				loadTextures();
		void				adaptPlayerPosition();
		void				adaptPlayerVelocity();
		void				handleCollisions();
		void				updateSounds();
		void				updateScore();

		void				buildScene();
		void				addEnemies();
		void				addEnemy(Aircraft::Type type, float relX, float relY);
		void				spawnEnemies();
		void				destroyEntitiesOutsideView();
		void				guideMissiles();
		sf::FloatRect		getViewBounds() const;
		sf::FloatRect		getBattlefieldBounds() const;

	private:
		enum class Layer
		{
			Background,
			LowerAir,
			UpperAir,
			LayerCount
		};

		struct SpawnPoint
		{
			SpawnPoint(Aircraft::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
			{
			}

			Aircraft::Type type;
			float x;
			float y;
		};

	private:
		sf::RenderWindow&					mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;
		FontHolder&							mFonts;
		SoundPlayer&						mSounds;

		SceneNode							mSceneGraph;
		std::array<SceneNode*, static_cast<std::size_t>(Layer::LayerCount)>		mSceneLayers;
		CommandQueue						mCommandQueue;

		sf::FloatRect						mWorldBounds;
		sf::Vector2f						mSpawnPosition;
		float								mScrollSpeed;
		Aircraft*							mPlayerAircraft;

		std::vector<SpawnPoint>				mEnemySpawnPoints;
		std::vector<Aircraft*>				mActiveEnemies;

		int									mScore;
		TextNode*							mScoreDisplay;
		sf::Vector2f						mScorePosition;
};

bool matchesCategories(std::pair<SceneNode*, SceneNode*>& colliders, Category category1, Category category2);

#endif // WORLD_HPP

