#include "Level.h"
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

Level::Level(sf::RenderWindow *pWindow) : _window(pWindow)
{
	_waves.push_back(new EnemyWave());
	_currentEnemyWave = _waves.at(_indexWave);

}

Level::~Level()
{
}

void Level::CreateWaypoint(sf::Vector2f pWayPos, float pSec)
{
	_waves.at(_indexWave)->addWaypoint(new Waypoint(pWayPos, _currentEnemyWave->getWaypoints()->size(), _waves.size(), _window), pSec);
}

void Level::StartLevel(World* pWorld)
{
	_world = pWorld;	
}

int Level::getIndexWave()
{
	return _indexWave;
}

//Return true when all the level waves have been completed
bool Level::RunLevel(sf::Time* pTime)
{
	_currentGameTime = pTime;
	_currentSecInGame = _currentGameTime->asSeconds();
	for (auto &enemyWave : _waves)
	{
		if (enemyWave->CheckSpawnTimeNextEnemy(&_currentSecInGame))
			enemyWave->SpawnEnemy(_world);
	}
	return false;
}

bool Level::RunLevel(float* pSec)
{
	for (auto &enemyWave : _waves)
	{
		if (enemyWave->CheckSpawnTimeNextEnemy(pSec))
			enemyWave->SpawnEnemy(_world);
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
	
	_indexWave++;
	cout << _indexWave << "<NEXT index " << endl;
	_currentEnemyWave = _waves.at(_indexWave);
	cout << _waves.size() << "<waves size " << endl;
	return _currentEnemyWave;
}

EnemyWave * Level::PreviousWave()
{

	_indexWave--;
	cout << _indexWave << "<PREVIOUS index " << endl;
	_currentEnemyWave = _waves.at(_indexWave);
	cout << _waves.size() << "<waves size " << endl;
	return _currentEnemyWave;
}

EnemyWave * Level::NewWave()
{
	_waves.push_back(new EnemyWave());
	_indexWave++;
	cout << _indexWave << "<NEW index " << endl;
	_currentEnemyWave = _waves.at(_indexWave);
	cout << _waves.size() << "<waves size " << endl;
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
		cout << "Can�t Erase only wave" << endl;
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

//TODO:
//Here do all the logic of the level, spawning waves, manages score, levels, etc
//Or do Level Manager Class where you just input the level and it runs all the info
//of the waves of the level and manage the score
