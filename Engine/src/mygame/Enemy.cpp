#include "Enemy.h"

Enemy::Enemy(std::string pName, glm::vec3 pPosition) : GameObject(pName,pPosition)
{
}

Enemy::~Enemy()
{
	//cout << "DESTROYED" << endl;
}

void Enemy::setEnemyType(int pType = 0)
{
	_enemyType = pType;
}
