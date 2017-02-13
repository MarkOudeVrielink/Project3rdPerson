#include "mgengine\Core\ControlledActor.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

ControlledActor::ControlledActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, float pMass, float pHealth, float pStrength)
	: Actor(pWorld, pName, pPosition, pCollider, pMass), _health(pHealth), _strength(pStrength)
{
	//initRigidBody(pCollider);
}

ControlledActor::~ControlledActor()
{
	
}

void ControlledActor::update(float pStep)
{
		
	AjustPosition();
	if (_actorBehaviour) _actorBehaviour->update(pStep);
}

float ControlledActor::GetHealth()
{
	return _health;
}

void ControlledActor::TakeDamage(float pDamage)
{
	_health -= pDamage;

	if (_health <= 0) {
		std::cout << "dieded" << std::endl;
		//this->~ControlledActor();
	}
}
