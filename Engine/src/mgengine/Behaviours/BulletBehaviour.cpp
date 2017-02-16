#include "BulletBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Behaviours\PlayerBehaviour.h"
#include <iostream>

BulletBehaviour::BulletBehaviour(float pSpeed, float pPower, float pLiveTime, Direction pDirection, BulletOwner pOwner) 
	: _speed(pSpeed), _power (pPower), _liveTime(pLiveTime), _direction(pDirection), _bulletOwner(pOwner)
{
	_timer = 0;
}

BulletBehaviour::~BulletBehaviour()
{
	
}

void BulletBehaviour::update(float pStep)
{
	if (_direction == Direction::Up) {
		_force = btVector3(0,0,-_speed);
	}
	else {
		_force = btVector3(0, 0, _speed);
	}
	
	_ownerBody->translate(_force);
	_timer++;
	
	if (_timer >= _liveTime) {
		_owner->~Actor();
	}
		
}

/*Using a down cast to get the right behaviour and actor. This won't work if the actor is not configured correctly. 
However, the actors in the scene should be set-up correct in any case.*/
void BulletBehaviour::OnCollision(Actor * pOther)
{	
	ActorType type = pOther->GetType();

	if (type == ActorType::Type_Enemy && _bulletOwner == BulletOwner::Player) {	
		ControlledActor* enemy = (ControlledActor*)pOther;
		enemy->TakeDamage(_power);
		_owner->~Actor();
	}
	else if (type == ActorType::Type_Player && _bulletOwner == BulletOwner::Enemy) {
		/*ControlledActor* player = (ControlledActor*)pOther;
		player->TakeDamage(_power);
		_owner->~Actor();*/
	}
	else if(type != ActorType::Type_Bullet){
		_owner->~Actor();
	}
	//TODO: Add sfx. "plink"?
}

BulletOwner BulletBehaviour::GetOwner()
{
	return _bulletOwner;
}
