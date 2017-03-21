#include "Level.h"
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
//EmptyLEvel
Level::Level()
{
	_indexWave = -1; //Safe check in case we do +1 in the index wave when loading xml
}
//TODO: Add setter for the sf::RenderWindow 
Level::Level(sf::RenderWindow *pWindow) : _window(pWindow)
{
	_waves.push_back(new EnemyWave());
	_currentEnemyWave = _waves.at(_indexWave);

}

Level::~Level()
{
}
//TODO:: Add current wave setSize
//TODO:: Add current wave setDelayBetweenEnemies
void Level::CreateWaypoint(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec)
{	
	//cout << pSec << "Time when adding waypoint" << endl;
	_waves.at(_indexWave)->addWaypoint(new Waypoint(pWolrdWaypointPos,pScreenWayPos, _currentEnemyWave->getWaypoints()->size(), _indexWave, _window), pSec);
}

void Level::CreateMainWaypointMoveDirection(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec)
{
	//cout << pSec << "Time when adding waypoint" << endl;
	_waves.at(_indexWave)->addMainWaypointDirection(new Waypoint(pWolrdWaypointPos, pScreenWayPos,000, _indexWave, _window), pSec);

}

//Add reference to the world
void Level::ReferenceWorld(World* pWorld, GameObject* pParent)
{
	if(this != nullptr)
		_world = pWorld;

	_gameObjectsParent = pParent;
}
void Level::ReferenceToParent(GameObject* pParent)
{
	_gameObjectsParent = pParent;
}
int Level::getIndexWave()
{
	return _indexWave;
}

//Return true when all the level waves have been completed <-ADD
bool Level::RunLevel(sf::Time* pTime)
{
	if (!_world->getDialogue(1))
	{
		_world->setDialogue(true, 1);
	}
	else if(_world->getDialogueEnded(1)){
		_currentGameTime = pTime;
		_currentSecInGame = _currentGameTime->asSeconds();
		for (auto &enemyWave : _waves)
		{
			if (enemyWave->CheckSpawnTimeNextEnemy(&_currentSecInGame))
			{
				if (*enemyWave->getEnemyBehaviour() == 2 && !_world->getDialogue(2))
				{
					_world->setDialogue(true, 2);
					_dialoguePreBos = true;
				}
				else if (_dialoguePreBos == true && _world->getDialogueEnded(2))
					enemyWave->SpawnEnemy(_world, _gameObjectsParent);
				else if (*enemyWave->getEnemyBehaviour() != 2)
					enemyWave->SpawnEnemy(_world, _gameObjectsParent);
			}
		}
	}
	return false;
}

bool Level::RunLevel(float* pSec)
{
	for (auto &enemyWave : _waves)
	{
		if (enemyWave->CheckSpawnTimeNextEnemy(pSec))
			enemyWave->SpawnEnemy(_world, _gameObjectsParent);
	}

	return false;
}

EnemyWave * Level::getCurrentWave()
{
	return _currentEnemyWave;
}
//Add breakpoint if we dont have enough waves
EnemyWave * Level::NextEnemyWave()
{
	if (_indexWave < _waves.size()-1)
	{
		_indexWave++;
		//cout << _indexWave << "<NEXT index " << endl;
		_currentEnemyWave = _waves.at(_indexWave);
		//cout << _waves.size() << "<waves size " << endl;
		return _currentEnemyWave;
	}
	else {
		//cout << "We should not keep going up in the wave" << endl;
		return 0;
	}
}

EnemyWave * Level::PreviousWave()
{
	if (_indexWave > 0)
	{
		_indexWave--;
		//cout << _indexWave << "<PREVIOUS index " << endl;
		_currentEnemyWave = _waves.at(_indexWave);
		//cout << _waves.size() << "<waves size " << endl;
		return _currentEnemyWave;
	}
	else 
	{
		//cout << "We should not keep going low in the wave" << endl;
		return 0;
	}
}

EnemyWave * Level::NewWave()
{
	_waves.push_back(new EnemyWave());
	_indexWave++;
	//cout << _indexWave << "<NEW index " << endl;
	_currentEnemyWave = _waves.at(_indexWave);
	//cout << _waves.size() << "<waves size " << endl;
	return _currentEnemyWave;
}

EnemyWave * Level::DeleteWave()
{
	if (_indexWave != 0) { //If we are not in wave 0 then we set the new wave as the previous one
		_waves.erase(_waves.begin() + _indexWave);
		_indexWave--;
		_currentEnemyWave = _waves.at(_indexWave);
		return _waves.at(_indexWave);
	}
	else if (_waves.size() > 1) { //If we are at wave 0 and we want to erase it then just set current wave the new wave that is at 0 position	
		_waves.erase(_waves.begin() + _indexWave);
		_currentEnemyWave = _waves.at(_indexWave);
		return _currentEnemyWave;
	}
	else {
		//cout << "Can´t Erase only wave" << endl;
		return nullptr;
	}
}

std::vector<EnemyWave*>* Level::getWaves()
{
	return &_waves;
}

void Level::DrawAllWavesWayPoints()
{
	for (auto wave : _waves)
	{
		if (wave == _currentEnemyWave)
			wave->setAsMainWave();
		else
			wave->setAsSecondaryWave();
		wave->DrawWaypoints();
	}
}

float Level::getCurrentSnapTime()
{
	return _currentSnapTime;
}

void Level::setCurrentSnapTime(float pSec)
{
	_currentSnapTime = pSec;
}

void Level::setRenderWindow(sf::RenderWindow* pWindow)
{
	_window = pWindow;
}

//TODO:
//Here do all the logic of the level, spawning waves, manages score, levels, etc
//Or do Level Manager Class where you just input the level and it runs all the info
//of the waves of the level and manage the score

