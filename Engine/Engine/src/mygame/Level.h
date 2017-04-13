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
	//void CreateWaypoint(sf::Vector2f pWayPos, float pSec);
	//void CreateWaypoint(sf::Vector3f pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec);
	void CreateWaypoint(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec);
	void CreateMainWaypointMoveDirection(glm::vec3 pWolrdWaypointPos, sf::Vector2f pScreenWayPos, float pSec);
	//void ReferenceWorld(World *pWorld);
	void ReferenceWorld(World * pWorld, GameObject * pParent);
	void ReferenceToParent(GameObject * pParent);
	int getIndexWave();
	bool RunLevel(sf::Time* pTime);
	bool RunLevel(float* pSec);
	EnemyWave* getCurrentWave();
	EnemyWave* NextEnemyWave();
	EnemyWave* PreviousWave();
	EnemyWave* NewWave();
	EnemyWave* DeleteWave();
	EnemyWave * ClearWave();
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
	GameObject* _gameObjectsParent;
	std::vector<EnemyWave*> _waves;
	std::string _LevelName = "Level...";
	int _indexWave = 0;
	float _currentSnapTime = 5;
	sf::Time * _currentGameTime;
	float  _currentSecInGame = 0;
	bool _dialoguePreGame = false;
	bool _dialoguePreBos = false;
	bool _dialoguePosWin = false;

};


