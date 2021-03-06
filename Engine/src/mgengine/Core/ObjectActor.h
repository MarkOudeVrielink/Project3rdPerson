#ifndef OBJECTACTOR_H
#define OBJECTACTOR_H

#include "mgengine\Core\Actor.h"


class ObjectActor : public Actor {
public :
	ObjectActor(World* pWorld, 
				std::string pName,
				glm::vec3 pPosition,
				btCollisionShape* pCollider = new btSphereShape(1),
				ActorType pType				= ActorType::Type_StaticObject,
				float pMass					= 1.0f);
	
	ObjectActor(World* pWorld,
				std::string pName,
				glm::vec3 pPosition,
				btCollisionShape* pCollider = new btSphereShape(1),
				ActorType pType				= ActorType::Type_StaticObject,
				short pCollisionGroup		= 0,
				short pCollisionMask		= 0,
				float pMass					= 1.0f);
	~ObjectActor();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pActor);

private:
	
};
#endif // !OBJECTACTOR_H