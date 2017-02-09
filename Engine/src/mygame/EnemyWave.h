#pragma once
#include <list>
#include "Enemy.h"
class EnemyWave
{
public:
	EnemyWave();
	~EnemyWave();
private:
	std::vector<Enemy*> _enemies;
	float _health;//standard health enemies
	bool _destroyed = false; //If the whole wave has been destroyed
	int _rewardWave; //TODO: Enum of rewards for the reward we get for destroying the whole wave
	std::vector<sf::Vector2i> _wayPoints;
};

