#include "CameraBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
CameraBehaviour::CameraBehaviour(float pMoveSpeed)
{
}

CameraBehaviour::~CameraBehaviour()
{
}

void CameraBehaviour::update(float pStep)
{
	float moveSpeed = 20.0f; //default if no keys
	float turnSpeed = 0.0f;
	///SpaceshipMovement
	_owner->translate(glm::vec3(0*pStep, moveSpeed*pStep, 0*pStep));

}
