#include "ObjectActor.h"

ObjectActor::ObjectActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, float pMass, ObjectType pType)
	: Actor(pWorld, pName, pPosition, pCollider, pMass), _type(pType)
{

}

ObjectActor::~ObjectActor()
{	
	RemoveRigidBodyFromWorld();
}

void ObjectActor::update(float pStep)
{
	if (_actorBehaviour) _actorBehaviour->update(pStep);
	AjustPosition();
}

ObjectType ObjectActor::GetType()
{
	return _type;
}
