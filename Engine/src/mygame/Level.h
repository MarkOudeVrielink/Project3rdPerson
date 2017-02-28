#pragma once
#include "mygame\EnemyWave.h"
#include <list>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics.hpp>
#include "Waypoint.h"
#include "mge\core\World.hpp"
class Level
{
public:
	Level();
	Level(sf::RenderWindow *pWindow);
	~Level();
	void CreateWaypoint(sf::Vector2f pWayPos, float pSec);
	void StartLevel(World *pWorld);
	int getIndexWave();
	bool RunLevel(sf::Time* pTime);
	bool RunLevel(float* pSec);
	EnemyWave* getCurrentWave();
	EnemyWave* NextEnemyWave();
	EnemyWave* PreviousWave();
	EnemyWave* NewWave();
	EnemyWave* DeleteWave();
	std::vector<EnemyWave*>* getWaves();
	void DrawAllWavesWayPoints();
	float getCurrentSnapTime();
	void setCurrentSnapTime(float pSec);
	void setRenderWindow(sf::RenderWindow* pWindow);
	//TODO ADD GET OF LEVEL NAME
private:
	EnemyWave* _currentEnemyWave;//Reference to the current wave 
	sf::RenderWindow* _window;
	World* _world;
	std::vector<EnemyWave*> _waves;
	std::string _LevelName = "Level...";
	int _indexWave = 0;
	float _currentSnapTime = 5;
	sf::Time * _currentGameTime;
	float  _currentSecInGame = 0;

};


