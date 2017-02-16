#pragma once
#include <list>
#include "Waypoint.h"
#include "Enemy.h"
#include "mge/core/World.hpp"
class EnemyWave
{
public:
	EnemyWave();
	~EnemyWave();
	void addWaypoint(Waypoint * pWaypoint, float pSec);
	std::vector<Waypoint*>* getWaypoints();
	void DrawWaypoints();
	void SpawnEnemy(World * pWorld);
	bool CheckSpawnTimeNextEnemy(float* pSec);
	void UpdateEditorModePosition(float *pSec);
	void TestRealTime();
	void TestEditorMode();
private:
	float _startTimeWave = 10000;
	int _sizeWave = 5;
	int _quantitySpawnedEnemies = 0; 
	float _timeAtLastEnemySpawned = 0; //Time in Seconds when the last enemy was spawned
	float _delayBetweenEnemies = .5f; //Delay between every enemy in the wave
	std::vector<Enemy*> _enemies;
	float _health;//standard health enemies
	bool _destroyed = false; //If the whole wave has been destroyed
	int _rewardWave; //TODO: Enum of rewards for the reward we get for destroying the whole wave
	std::vector<Waypoint*> _wayPoints;

	float* _snapTime = 0;
	bool _editorMode = true;

};

