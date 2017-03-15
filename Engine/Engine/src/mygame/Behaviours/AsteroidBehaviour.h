#pragma once
#include "mge\Behaviours\AbstractBehaviour.hpp"
#include "mge\core\GameObject.hpp"
class AsteroidBehaviour : public AbstractBehaviour {
public:
	AsteroidBehaviour();
	virtual void update(float pStep);
private:
	glm::vec3 _axis;
};