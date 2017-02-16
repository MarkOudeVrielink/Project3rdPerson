#include "mgengine\Behaviours\PickUpBehaviour.h"
#include"mgengine\Core\Actor.h"

PickUpBehaviour::PickUpBehaviour(float pDriftSpeed, float pPoints, float pCharge) 
	: _speed(pDriftSpeed), _points(pPoints), _charge(pCharge)
{
}

PickUpBehaviour::~PickUpBehaviour()
{
}

void PickUpBehaviour::update(float pStep)
{
	_ownerBody->applyForce(btVector3(0,0,_speed), btVector3(0, 0, _speed));	
}

void PickUpBehaviour::OnCollision(Actor * pOther)
{
	if (pOther->GetType() == ActorType::Type_Player) {
		_owner->~Actor();
	}
}

float PickUpBehaviour::GetPoints()
{
	return _points;
}

float PickUpBehaviour::GetCharge()
{
	return _charge;
}
