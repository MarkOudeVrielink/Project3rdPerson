#include "ObjectActor.h"
#include "mge\core\World.hpp"

/*Create a deafault ObjectActor.*/
ObjectActor::ObjectActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, float pMass)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pMass)
{

}

/*Creates an actor that collides with what the specified group/mask is supposed to collide with.
E.g. group: player matches with mask: player. This way the player collides with the type of objects it's supposed to collide with.
*/
ObjectActor::ObjectActor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass)
{
}

ObjectActor::~ObjectActor()
{	
	_removeRigidBodyFromWorld();
}

void ObjectActor::update(float pStep)
{
	if (_actorBehaviour) _actorBehaviour->update(pStep);
	_ajustPosition();
}

void ObjectActor::OnCollision(Actor * pActor)
{
	if (_actorBehaviour)
		_actorBehaviour->OnCollision(pActor);
}

void ObjectActor::Reset()
{
	Actor::Reset();
}

void ObjectActor::ReCreate(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass)
{
	Actor::ReCreate(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass);
}

void ObjectActor::Destroy()
{
	_setDirty();
}

void ObjectActor::_setDirty()
{
	//_world->SetDirtyActor(this);
	_world->ResetObject(this);
}
