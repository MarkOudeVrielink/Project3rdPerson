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

	void DrawWaypoints();
	void setAsMainWave();
	void setAsSecondaryWave();
	void SpawnEnemy(World * pWorld);
	bool CheckSpawnTimeNextEnemy(float* pSec);
	void UpdateEditorModePosition(float *pSec);
	void TestRealTime();
	void TestEditorMode();

	const float* getStartTime() const;
	const int* getSizeWave() const;
	const float* getDelayBetweenEnemies() const;
	const float* getSpeed() const;
	const float* getShootRatio() const;
	const int* getEnemyType() const;
	const int* getEnemyBehaviour() const;
	const float* getHealth() const;
	const std::vector<Waypoint*>* getWaypoints() const;

	void setWaypoints(std::vector<Waypoint*> pWaypoints);
	void setStartTime(float pStartTime);
	void setSizeWave(int pSizeWave);
	void setDelayBetweenEnemies(float pDelayEnemies);
	void setSpeed(float pSpeed);
	void setShootRatio(float pRatio);
	void setEnemyType(float pType);
	void setEnemyBehaviour(float pBehaviour);
	void setHealth(float pHealth);
private:
	float _startTimeWave = 10000;
	int _sizeWave = 5;
	int _quantitySpawnedEnemies = 0;
	float _timeAtLastEnemySpawned = 0; //Time in Seconds when the last enemy was spawned
	float _delayBetweenEnemies = .5f; //Delay between every enemy in the wave
	std::vector<Enemy*> _enemies;
	//float _health;//standard health enemies
	bool _destroyed = false; //If the whole wave has been destroyed
	int _rewardWave; //TODO: Enum of rewards for the reward we get for destroying the whole wave
	std::vector<Waypoint*> _wayPoints;

	float* _snapTime = 0;
	Mesh* _enemy;
	bool _editorMode = true;
	float _speed = 20;
	float _shootRatio = 0;
	int _enemyType = 0;
	int _enemyBehaviour = 0;
	float _health = 1;

	//bool _isMainWave = false;

};

