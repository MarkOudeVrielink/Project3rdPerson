#include "AsteroidBehaviour.h"

AsteroidBehaviour::AsteroidBehaviour() :AbstractBehaviour()
{
	_axis = glm::vec3(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1));
}

void AsteroidBehaviour::update(float pStep)
{
	_owner->rotate(pStep * glm::radians(45.0f), glm::vec3(_axis));
}
