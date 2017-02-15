#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Behaviours\EnemyBehaviour.h"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

ControlledActor::ControlledActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, ActorType pType, float pMass, float pHealth, float pStrength)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pMass), _health(pHealth), _strength(pStrength)
{
	//initRigidBody(pCollider);	
}

ControlledActor::~ControlledActor()
{
	
}

void ControlledActor::update(float pStep)
{		
	AjustPosition();
	if (_actorBehaviour)  _actorBehaviour->update(pStep); 
}

void ControlledActor::OnCollision(Actor * pActor)
{
	if (_actorBehaviour)
		_actorBehaviour->OnCollision(pActor);
}

float ControlledActor::GetHealth()
{
	return _health;
}

void ControlledActor::TakeDamage(float pDamage)
{
	_health -= pDamage;
	
	if (_health <= 0) {
		if (_type == ActorType::Type_Enemy) {
			EnemyBehaviour* behavior = (EnemyBehaviour*)_actorBehaviour;
			behavior->SpawnDrop();//INPUT amount of drops enemies drop.
			delete this;
		}
		else {
			delete this;
		}
	}
}
