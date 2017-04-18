#ifndef PICKUPBEHAVIOUR_H
#define PICKUPBEHAVIOUR_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

class PickUpBehaviour : public AbstractActorBehaviour {
public:
	PickUpBehaviour(float pDriftSpeed = 1.0f, float pPoints = 5.0f, float pCharge = 1.0f);
	~PickUpBehaviour();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	float GetPoints();
	float GetCharge();
private:

	float _speed;
	float _points;
	float _charge;
	float _rotation = 0;
	int _random;
};

#endif // !PICKUPBEHAVIOUR_H


