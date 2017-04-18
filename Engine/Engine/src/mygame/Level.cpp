#include "Level.h"
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include "mge/config.hpp"
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
	if (!_font.loadFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
		cout << _font.getInfo().family << "Could not load font, exiting..." << endl;

	}
}

Level::~Level()
{
	for (auto wave : _waves)
	{
		delete wave;// = NULL;
	}


}
//TODO:: Add current wave setSize
//TODO:: Add current wave setDelayBetweenEnemies
void Level::CreateWaypoint(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec)
{
	//cout << pSec << "Time when adding waypoint" << endl;
	_waves.at(_indexWave)->addWaypoint(new Waypoint(pWolrdWaypointPos, pScreenWayPos, _currentEnemyWave->getWaypoints()->size(), _indexWave,_font, _window), pSec);
}

void Level::CreateMainWaypointMoveDirection(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec)
{
	//cout << pSec << "Time when adding waypoint" << endl;
	_waves.at(_indexWave)->addMainWaypointDirection(new Waypoint(pWolrdWaypointPos, pScreenWayPos, 000, _indexWave, _font, _window), pSec);

}

//Add reference to the world
void Level::ReferenceWorld(World* pWorld, GameObject* pParent)
{
	if (this != NULL)
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
{//Pause probably goes here
	
	//First dialogue that is show before the game starts
	if (!_world->getDialogue(1))
	{
		_world->setDialogue(true, 1);
		if (!startTimer)
		{
			startTimer = true;
			_timeWhenDialogueAppear = pTime->asSeconds();
			
		}
	}
	else if (_world->getDialogueEnded(1)) {
		//MAY BE super expensive
		
		if (startTimer && !dialogue1)
		{
			dialogue1 = true;
			startTimer = false;			
			_differenceTimeDialogueAndReal += pTime->asSeconds() - _timeWhenDialogueAppear;
			_timeWhenDialogueAppear = pTime->asSeconds();
		}
		_currentGameTime = pTime;
		_currentSecInGame = _currentGameTime->asSeconds();
		realTime = _currentSecInGame;// -_differenceTimeDialogueAndReal; //FIX
		for (auto &enemyWave : _waves)
		{
			
			//todo: check every wave enemybehaviuor, if is gonna spawn and no enemy of the same type has spawn then stop game and show popup
			if (enemyWave->CheckSpawnTimeNextEnemy(&realTime))
			{
				if ((CheckIfSpawnBefore(*enemyWave->getEnemyType()) && _world->getEnemyBioEnded(*enemyWave->getEnemyType() - 1)))
				{
					//cout << *enemyWave->getEnemyType() << endl;
					if (startTimer)
					{
						startTimer = false;
						_differenceTimeDialogueAndReal += pTime->asSeconds() - _timeWhenDialogueAppear;
						_timeWhenDialogueAppear =  pTime->asSeconds();
					}
					if (*enemyWave->getEnemyBehaviour() == 2 && !_world->getDialogue(2))
					{
						if (!startTimer)
						{
							startTimer = true;
							_timeWhenDialogueAppear = pTime->asMilliseconds();
						}
						_world->setDialogue(true, 2);
						_dialoguePreBos = true;
					}
					else if (_dialoguePreBos == true && _world->getDialogueEnded(2))
					{
						if (startTimer)
						{
							startTimer = false;			
							_differenceTimeDialogueAndReal += pTime->asSeconds() - _timeWhenDialogueAppear;
							_timeWhenDialogueAppear =  pTime->asSeconds();
						}
						enemyWave->SpawnEnemy(_world, _gameObjectsParent);
					}
					else if (*enemyWave->getEnemyBehaviour() != 2)
					{
						enemyWave->SpawnEnemy(_world, _gameObjectsParent);
						if (startTimer)
						{
							startTimer = false;
							_differenceTimeDialogueAndReal += pTime->asSeconds() - _timeWhenDialogueAppear;
							_timeWhenDialogueAppear = pTime->asSeconds();
						}
					}
				}
				else //If no enemies of that type spawn before, get "stop time" and show presentation
				{
					if (!startTimer)
					{
						startTimer = true;
						_timeWhenDialogueAppear = pTime->asSeconds();
					}
					realTime = _timeWhenDialogueAppear;
					setSpawnBefore(*enemyWave->getEnemyType(), true);
					_world->setEnemyBio(true, *enemyWave->getEnemyType() - 1);
					break;
				}
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
//Check if it´s the first time an enemy of this type appears
bool Level::CheckIfSpawnBefore(int pType)
{

	switch (pType)
	{
	case 2:
		return _enemyType1;
	case 3:
		return _enemyType2;
	case 4:
		return _enemyType3;
	case 5:
		return _enemyType4;
	case 6:
		return _enemyType5;
	case 7:
	case 8:
		return _enemyType6;
	}

	return true;
}
bool Level::setSpawnBefore(int pType, bool pBool)
{

	switch (pType)
	{
	case 2:
		_enemyType1 = pBool;
	case 3:
		_enemyType2 = pBool;
	case 4:
		_enemyType3 = pBool;
	case 5:
		_enemyType4 = pBool;
	case 6:
		_enemyType5 = pBool;
	case 7:
	case 8:
		_enemyType6 = pBool;
	}

	return true;
}
EnemyWave * Level::getCurrentWave()
{
	return _currentEnemyWave;
}
//Add breakpoint if we dont have enough waves
EnemyWave * Level::NextEnemyWave()
{
	if (_indexWave < _waves.size() - 1)
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
EnemyWave * Level::ClearWave()
{
	_currentEnemyWave->ClearWaypoints();
	return _waves.at(_indexWave);


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

