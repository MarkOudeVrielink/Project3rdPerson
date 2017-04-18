#include "mgengine\Core\ControlledActor.h"
#include "mygame\Behaviours\EnemyBehaviour.h"
#include "mge\core\World.hpp"

/*Create a deafault ControlledActor.*/
ControlledActor::ControlledActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, ActorType pType, float pMass, float pHealth, float pStrength)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pMass), _health(pHealth), _strength(pStrength)
{

}

/*Creates an actor that collides with what the specified group/mask is supposed to collide with.
E.g. group: player matches with mask: player. This way the player collides with the type of objects it's supposed to collide with.
*/
ControlledActor::ControlledActor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, float pMass, short pCollisionGroup, short pCollsionMask, float pHealth, float pStrength)
	: Actor(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollsionMask, pMass), _health(pHealth), _strength(pStrength)
{
}

ControlledActor::~ControlledActor()
{
	
}

void ControlledActor::update(float pStep)
{		
	_ajustPosition();
	if (_actorBehaviour)  
		_actorBehaviour->update(pStep); 
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
void ControlledActor::SetHealth(float pHealth)
{
	_health = pHealth;
}

void ControlledActor::TakeDamage(float pDamage)
{
	_health -= pDamage;		
}

void ControlledActor::Reset()
{
	Actor::Reset();

	_health = 0;
	_strength = 0;
}

void ControlledActor::ReCreate(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass, float pHealth, float pPower)
{
 	Actor::ReCreate(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass);
	std::cout << "group + mask " << pCollisionGroup << " " << pCollisionMask << std::endl;
	_health = pHealth;
	_strength = pPower;
}

void ControlledActor::Destroy()
{
	_setDirty();
}

void ControlledActor::_setDirty()
{
	//_world->SetDirtyActor(this);
	_world->ResetObject(this);
}
