#pragma once


#include "mge/behaviours/AbstractBehaviour.hpp"

class CameraBehaviour : public AbstractBehaviour
{
public:

	CameraBehaviour(float pMoveSpeed);
	~CameraBehaviour();

	virtual void update(float pStep);

private:
	float _moveSpeed;
	float _turnSpeed;
};

