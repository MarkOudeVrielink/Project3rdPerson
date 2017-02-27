#include "mgengine\Core\ControlledActor.h"
#include "mygame\Behaviours\EnemyBehaviour.h"

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
	AjustPosition();
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
