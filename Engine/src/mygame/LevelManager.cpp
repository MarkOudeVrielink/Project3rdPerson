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
			//delete asteroidParent; //TODO: find out what is wrong.
			asteroidParent = NULL;
		}
		/*for (auto asteroidPointer : _asteroids)
		{
			asteroidPointer = NULL;
		}*/

		if (_planet) {
			//delete _planet;
			_planet = nullptr;
		}

		if (_window)
			_window = nullptr;

		if (_backgroundPlane_0) {
			_backgroundPlane_0 = nullptr;
			_backgroundPlane_1 = nullptr;
			_backgroundPlane_2 = nullptr;
			_backgroundPlane_3 = nullptr;
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

		asteroidParent->translate(glm::vec3(0, 0, pStep * 10));		
		_updateBackground(pStep);

	//	cout << "working" << endl;
	}
}

void LevelManager::StartGameFromMenu()
{

	//cout << "Reading XML..." << endl;
	Level* level = LevelParser::LoadLevel(std::to_string(1), NULL);

	_currentLevel = level;
	_currentLevel->ReferenceWorld(_world, _owner);
	_currentLevel->RunLevel(&_time.getElapsedTime());
	_startGame = true;
	//_currentWave = _currentLevel->getIndexWave();

	//cout << "Done Reading XML..." << endl;
	InitializeBackground();

}

void LevelManager::InitializeBackground()
{

#pragma region background planes

	//_backgroundPlane_0 = new GameObject("background plane", glm::vec3(0, -550, 0));
	//_backgroundPlane_0->scale(glm::vec3(15, 15, 15));
	//_backgroundPlane_0->setMesh(_world->GetResourceManager()->getMesh(Meshes::BackGround));
	//_backgroundPlane_0->setMaterial(_world->GetResourceManager()->getMaterial(Materials::BackGround_0));
	//_world->add(_backgroundPlane_0);

	_backgroundPlane_1 = new GameObject("background plane", glm::vec3(0, -560, 0));
	_backgroundPlane_1->scale(glm::vec3(20, 10, 10));
	_backgroundPlane_1->setMesh(_world->GetResourceManager()->getMesh(Meshes::BackGround));
	_backgroundPlane_1->setMaterial(_world->GetResourceManager()->getMaterial(Materials::BackGround_1));
	_world->add(_backgroundPlane_1);

	_backgroundPlane_2 = new GameObject("background plane", glm::vec3(0, -400, 0));
	_backgroundPlane_2->scale(glm::vec3(20, 10, 10));
	_backgroundPlane_2->setMesh(_world->GetResourceManager()->getMesh(Meshes::BackGround));
	_backgroundPlane_2->setMaterial(_world->GetResourceManager()->getMaterial(Materials::BackGround_2));
	_world->add(_backgroundPlane_2);

	_backgroundPlane_3 = new GameObject("background plane", glm::vec3(0, -300, 0));
	_backgroundPlane_3->scale(glm::vec3(15, 10, 10));
	_backgroundPlane_3->setMesh(_world->GetResourceManager()->getMesh(Meshes::BackGround));
	_backgroundPlane_3->setMaterial(_world->GetResourceManager()->getMaterial(Materials::BackGround_3));
	_world->add(_backgroundPlane_3);
	
#pragma endregion

	InitializeAsteroids();
	/*_planet = new GameObject("Planet Object");
	_planet->scale(glm::vec3(10, 10, 10));
	_planet->setMesh(_world->GetResourceManager()->getMesh(Meshes::Planet));
	_planet->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Planet));
	_planet->setBehaviour(new AsteroidBehaviour());*/
	//_world->add(_planet);
	
}

void LevelManager::_updateBackground(float pStep)
{
	////TODO: stop when border is reached.
	//if (_time.getElapsedTime().asSeconds() < 500) {
		//_backgroundPlane_0->translate(glm::vec3(0, 0, pStep / 16));
		_backgroundPlane_1->translate(glm::vec3(0, 0, pStep / 12));
		_backgroundPlane_2->translate(glm::vec3(0, 0, pStep / 8));
		_backgroundPlane_3->translate(glm::vec3(0, 0, pStep / 6));
	//}
}

void LevelManager::InitializeAsteroids()
{
	asteroidParent = new GameObject("asteroidParent", glm::vec3(0, 0, 0));	
	for (int y = 1; y < 12;y++)
	{		
		for (int x = 0; x < 10;x++)
		{			
			for (int z = 0; z < 30;z++)
			{				
				if (std::rand() % 30 == 0)
				{
					printf("asteriod made");
					GameObject* asteroid = new GameObject("asteroid", glm::vec3(x * 40 - 130, -10 * y, 50 - z * 40));//Magic numbers in order to have asteroids in all the screen well distributed
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
