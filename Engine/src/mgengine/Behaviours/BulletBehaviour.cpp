#include "BulletBehaviour.h"
#include "mge\core\World.hpp"
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
	
	_ownerBody->translate(_force);	
	_timer += pStep;	

	if (_timer >= _liveTime) {
		_owner->Destroy();
	}

	if (_ownerType == ActorType::Type_Nova) {
		_animation->update(pStep);
	}
		
}

/*Using a down cast to get the right behaviour and actor. This won't work if the actor is not configured correctly. 
However, the actors in the scene should be set-up correct in any case.*/
void BulletBehaviour::OnCollision(Actor * pOther)
{		
	ActorType type = pOther->getType();
		
	if(type != ActorType::Type_Bullet && _owner->getType() != ActorType::Type_Nova){
		_owner->Destroy();		
	}
	
	if (type == ActorType::Type_Bullet) {
		_owner->Destroy();
	}
}

void BulletBehaviour::setup() {
	_ownerType = _owner->getType();

	if (_ownerType == ActorType::Type_Nova) {
		
		_nova.loadFromFile("mge/textures/ult.png");

		_animation = new Animation(_owner->getWorld()->getRenderWindow(), _nova, sf::Vector2i(661, 377), 15.0f, 0.5f);
		_animation->setScale(sf::Vector2f(3.0f, 3.0f));

		sf::Vector2u window = sf::Vector2u(_owner->getWorld()->getRenderWindow()->getSize());
		_animation->setPosition(sf::Vector2f(window.x/2, window.y/2));
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

glm::vec3 BulletBehaviour::getBulletDirection()
{
	return _bulletDirection;
}

