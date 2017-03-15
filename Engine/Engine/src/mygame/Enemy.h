#pragma once
using namespace std;
#include <glm.hpp>

#include "mge/core/GameObject.hpp"
#include <list>
#include <SFML\System\Vector2.hpp>
class Enemy: public GameObject
{
public:
	Enemy(std::string pName = "camera",
		glm::vec3 pPosition = glm::vec3(0.0f, 3.0f, 5.0f));
	~Enemy();
	void setEnemyType(int pType);
private:
	float _health;
	std::vector<sf::Vector2i> _wayPoints;
	bool _shoot = false;
	float _shootingRatio = 0.0f;
	glm::vec2 _direction = glm::vec2(0.0f, 1.0f);//Y is Z in 3 coordinate system
	int _bulletType;//TODO: Change to Enum with all the bullet types
	int _rewardType;//TDOO: Change to Enum...
	int _rewardRatio;//TDOO: Change to Enum...
	int _enemyType; //TDOO: Change to Enum...
///	EnemyBehavour behave; //TODO: Create Enemies Behaviours
//	glm::vec3 _rotationForce; //constant rotation
//	std::string _soundShoot;
//	std::string _soundDeath;
};

