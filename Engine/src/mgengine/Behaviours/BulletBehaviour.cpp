#include "BulletBehaviour.h"
#include "mgengine\Core\Actor.h"
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

BulletOwner BulletBehaviour::GetOwner()
{
	return _bulletOwner;
}
