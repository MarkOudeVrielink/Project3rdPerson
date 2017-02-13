#ifndef CONTROLLEDACTOR_H
#define CONTROLLEDACTOR_H

#include "mgengine\Core\Actor.h"
#include <glm.hpp>

class ControlledActor : public Actor {
public:
	ControlledActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, float pMass = 0, float pHealth = 10.0f, float pStrength = 1.0f);
	~ControlledActor();

	virtual void update(float pStep);

	float GetHealth();
	void TakeDamage(float pDamage);

private:
	float _health;
	float _strength;
};

#endif // !CONTROLLEDACTOR_H

