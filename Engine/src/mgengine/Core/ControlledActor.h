#ifndef CONTROLLEDACTOR_H
#define CONTROLLEDACTOR_H

#include "mgengine\Core\Actor.h"
#include <glm.hpp>

class ControlledActor : public Actor {
public:
	ControlledActor(World* pWorld, 
					std::string pName, 
					glm::vec3 pPosition, 
					btCollisionShape* pCollider = new btSphereShape(1), 
					ActorType pType				= ActorType::Type_Enemy, 
					float pMass					= 1.0f, 
					float pHealth				= 1.0f, 
					float pStrength				= 1.0f);

	ControlledActor(World* pWorld,
					std::string pName,
					glm::vec3 pPosition,
					btCollisionShape* pCollider = new btSphereShape(1),
					ActorType pType				= ActorType::Type_Enemy,
					float pMass					= 1.0f,
					short pCollisionGroup		= 0,
					short pCollsionMask			= 0,
					float pHealth				= 1.0f,
					float pStrength				= 1.0f);
	~ControlledActor();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pActor);

	float GetHealth();
	void SetHealth(float pHealth);
	void TakeDamage(float pDamage);

private:
	float _health;
	float _strength;
};

#endif // !CONTROLLEDACTOR_H

