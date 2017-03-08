#include "LevelManager.h"
#include "mgengine\Parser\LevelParser.h"
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include "mygame\Behaviours\AsteroidBehaviour.h"
//TODO:
//Here do all the logic of the level, spawning waves, manages score, levels, etc
//Or do Level Manager Class where you just input the level and it runs all the info
//of the waves of the level and manage the score

//Level manager necessary because is the on who goes inside the update loop
LevelManager::LevelManager(World *pWorld) : _world(pWorld)
{
	_time = sf::Clock();
}

LevelManager::~LevelManager()
{
	try {
		if (_currentLevel) {
			delete _currentLevel;
			_currentLevel = NULL;
		}
		if (asteroidParent != NULL) {
			delete asteroidParent;
			asteroidParent = NULL;
		}
		/*for (auto asteroidPointer : _asteroids)
		{
			asteroidPointer = NULL;
		}*/
		if (_planet) {
			delete _planet;
		}
		if (_window)
			_window = nullptr;
		if (_backgroundPlane) {
			//delete _material;
			delete _backgroundPlane;
		}

		if (_world) {
			// _world;
			_world = NULL;
		}
	}
	catch(int e)
	{ }

}

//The level is initialized... All the logic about the game/level, here is managed all the information of 
//the levels, including waves, and enemies data
void LevelManager::StartLevel()
{
	if (_startGame)
		_currentLevel->ReferenceWorld(_world,_owner);
}

int LevelManager::getIndexLevel()
{
	return _indexLevel;
}

void LevelManager::update(float pStep)
{
	if (_startGame) {
		_currentLevel->RunLevel(&_time.getElapsedTime());
		//_backgroundPlane->translate(glm::vec3(0, 0, pStep / 4));

		asteroidParent->translate(glm::vec3(0, 0, pStep * 10));
	//	cout << "working" << endl;

	}
}

void LevelManager::StartGameFromMenu()
{
	cout << "Reading XML..." << endl;
	Level* level = LevelParser::LoadLevel(std::to_string(1), NULL);
	_currentLevel = level;
	_currentLevel->ReferenceWorld(_world, _owner);
	_currentLevel->RunLevel(&_time.getElapsedTime());
	_startGame = true;
	//_currentWave = _currentLevel->getIndexWave();
	cout << "Done Reading XML..." << endl;
	InitializeBackground();

}
void LevelManager::InitializeBackground()
{
	_backgroundPlane = new GameObject("background plane", glm::vec3(0, -500, 0));
	//_backgroundPlane->scale(glm::vec3(40, 40, 40));
	_backgroundPlane->setMesh(_world->GetResourceManager()->getMesh(Meshes::BackGround));
	_backgroundPlane->setMaterial(_world->GetResourceManager()->getMaterial(Materials::BackGround));
	_world->add(_backgroundPlane);
	_planet = new GameObject("Planet Object");
	_planet->scale(glm::vec3(10, 10, 10));
	_planet->setMesh(_world->GetResourceManager()->getMesh(Meshes::Planet));
	_planet->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Planet));
	_planet->setBehaviour(new AsteroidBehaviour());
	//_world->add(_planet);
	InitializeAsteroids();
}
void LevelManager::UpdateBAckground()
{

}
void LevelManager::InitializeAsteroids()
{
	asteroidParent = new GameObject("asteroidParent", glm::vec3(0, 0, 0));
	for (int y = 1; y < 6;y++)
	{
		for (int x = 0; x < 40;x++)
		{
			for (int z = 0; z < 100;z++)
			{
				if (std::rand() % 60 == 0)
				{
					GameObject * asteroid = new GameObject("asteroid", glm::vec3(x * 10 - 130, -40 * y, 50 - z * 10));//Magic numbers in order to have asteroids in all the screen well distributed
					asteroid->setMesh(_world->GetResourceManager()->getMesh(Meshes::Meteor));
					asteroid->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Meteor));
					asteroid->setBehaviour(new AsteroidBehaviour());

					asteroidParent->add(asteroid);
					_asteroids.push_back(asteroid);
				}
			}

		}
	}
	_world->add(asteroidParent);
}