#pragma once


#include "mge/behaviours/AbstractBehaviour.hpp"

class SpaceShipBehaviour : public AbstractBehaviour
{
public:

	SpaceShipBehaviour(float pMoveSpeed, float pTurnSpeed);
	~SpaceShipBehaviour();
	
	virtual void update(float pStep);

private:
	float _moveSpeed = 20.0f;
	float _sideSpeed = 20.0f;
};

