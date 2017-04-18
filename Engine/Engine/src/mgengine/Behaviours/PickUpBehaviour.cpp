#include "mgengine\Behaviours\PickUpBehaviour.h"
#include"mgengine\Core\Actor.h"
#include <random>
#include "glm.hpp"

PickUpBehaviour::PickUpBehaviour(float pDriftSpeed, float pPoints, float pCharge)
	: _speed(pDriftSpeed), _points(pPoints), _charge(pCharge)
{
	//random number between 0 and 6
	_random = rand() % 7;
}

PickUpBehaviour::~PickUpBehaviour()
{
}

void PickUpBehaviour::update(float pStep)
{
	_rotation += pStep*2;
	_ownerBody->applyForce(btVector3(0, 0, _speed), btVector3(0, 0, _speed));
	switch (_random) {
	case 0:
		_owner->setRotation(glm::vec3(1, 0, 0), _rotation);
		break;
	case 1:
		_owner->setRotation(glm::vec3(0, 1, 0), _rotation);
		break;
	case 2:
		_owner->setRotation(glm::vec3(0, 0, 1), _rotation);
		break;
	case 3:
		_owner->setRotation(glm::vec3(1, 1, 0), _rotation);
		break;
	case 4:
		_owner->setRotation(glm::vec3(1, 0, 1), _rotation);
		break;
	case 5:
		_owner->setRotation(glm::vec3(0, 1, 1), _rotation);
		break;
	case 6:
		_owner->setRotation(glm::vec3(1, 1, 1), _rotation);
		break;

	}
}

void PickUpBehaviour::OnCollision(Actor * pOther)
{
	if (pOther->getType() == ActorType::Type_Player) {
		_owner->Destroy();
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
