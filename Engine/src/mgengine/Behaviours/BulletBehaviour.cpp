#include "BulletBehaviour.h"
#include "mge\core\World.hpp"
#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Behaviours\PlayerBehaviour.h"
#include "glm.hpp"
#include <iostream>

BulletBehaviour::BulletBehaviour(float pSpeed, float pPower, float pLiveTime, Direction pDirection, BulletOwner pOwner) 
	: _speed(pSpeed), _power (pPower), _liveTime(pLiveTime), _direction(pDirection), _bulletOwner(pOwner), _timer(0)
{	
}

BulletBehaviour::~BulletBehaviour()
{
	
}

void BulletBehaviour::update(float pStep)
{	
	if (_direction == Direction::Up) {
		_force = btVector3(0,0,-_speed);
	}
	else if(_direction == Direction::Down){
		_force = btVector3(0, 0, _speed);
	}

	else
	{		
		_force = btVector3(_bulletDirection.x, _bulletDirection.y, _bulletDirection.z)*_speed;
	}
	//_force = btVector3(glm::sin(pStep) *glm::cos(pStep)*glm::log(abs(pStep)), 0,
//	(glm::pow(glm::pow((pStep), 2), (3 / 20))* glm::sqrt(glm::cos(pStep))));
	_ownerBody->translate(_force);	
	_timer += pStep;	

	if (_timer >= _liveTime) {
		_owner->Destroy();
	}
		
}

/*Using a down cast to get the right behaviour and actor. This won't work if the actor is not configured correctly. 
However, the actors in the scene should be set-up correct in any case.*/
void BulletBehaviour::OnCollision(Actor * pOther)
{		
	ActorType type = pOther->getType();
		
	if (type == ActorType::Type_Player && _bulletOwner == BulletOwner::Enemy) {//TODO:: move this to the player.
		_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Hit);
		
		ControlledActor* player = (ControlledActor*)pOther;
		player->TakeDamage(_power);
		_owner->Destroy();
	}	
	else if(type != ActorType::Type_Bullet && _owner->getType() != ActorType::Type_Nova){
		_owner->Destroy();		
	}
	
}

BulletOwner BulletBehaviour::getOwner()
{
	return _bulletOwner;
}

float BulletBehaviour::getPower()
{
	return _power;
}

void BulletBehaviour::setBulletDirection(glm::vec3 pDirection)
{
	_bulletDirection = pDirection;
}
void BulletBehaviour::setBulletRotation(glm::vec3 pAxis, float pAngle)
{
	_owner->SetRotation(pAxis,pAngle);
}

glm::vec3 BulletBehaviour::getBulletDirection()
{
	return _bulletDirection;
}

