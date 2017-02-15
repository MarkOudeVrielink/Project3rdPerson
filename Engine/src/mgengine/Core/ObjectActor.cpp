#include "ObjectActor.h"

ObjectActor::ObjectActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, float pMass)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pMass)
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

void ObjectActor::OnCollision(Actor * pActor)
{
	if (_actorBehaviour)
		_actorBehaviour->OnCollision(pActor);
}
