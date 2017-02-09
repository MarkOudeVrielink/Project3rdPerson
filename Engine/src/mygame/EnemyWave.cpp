#include "EnemyWave.h"

EnemyWave::EnemyWave()
{

	Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
	_enemies.push_back(Enemy1);
}


EnemyWave::~EnemyWave()
{
}
