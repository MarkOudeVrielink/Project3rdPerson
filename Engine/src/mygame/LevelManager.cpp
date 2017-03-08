#include "LevelManager.h"
#include "mgengine\Parser\LevelParser.h"
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
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
}

//The level is initialized... All the logic about the game/level, here is managed all the information of 
//the levels, including waves, and enemies data
void LevelManager::StartLevel()
{
	if(_startGame)
	_currentLevel->ReferenceWorld(_world);
}

int LevelManager::getIndexLevel()
{
	return _indexLevel;
}

void LevelManager::update(float pStep)
{
	if(_startGame)
	_currentLevel->RunLevel(&_time.getElapsedTime());
}

void LevelManager::StartGameFromMenu()
{
	//cout << "Reading XML..." << endl;
	Level* level = LevelParser::LoadLevel(std::to_string(1),NULL);
	_currentLevel = level;
	_currentLevel->ReferenceWorld(_world);
	_currentLevel->RunLevel(&_time.getElapsedTime());
	_startGame = true;
	//_currentWave = _currentLevel->getIndexWave();
	//cout << "Done Reading XML..." << endl;

}