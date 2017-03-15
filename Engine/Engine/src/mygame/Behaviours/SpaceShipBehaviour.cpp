#include "SpaceShipBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
SpaceShipBehaviour::SpaceShipBehaviour(float pMoveSpeed, float pTurnSpeed) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _sideSpeed(pTurnSpeed)
{
}

SpaceShipBehaviour::~SpaceShipBehaviour()
{
}
void SpaceShipBehaviour::update(float pStep)
{
	float moveSpeed = 0.0f; //default if no keys
	float sideSpeed = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveSpeed = _moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveSpeed = -_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		sideSpeed = -_sideSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		sideSpeed = _sideSpeed;
	}
	//translate the object in its own local space
	//_owner->translate( glm::vec3(0.0f, 0.0f, moveSpeed*pStep ) );

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, than we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);
	//


	///SpaceshipMovement
	_owner->translate(glm::vec3(sideSpeed*pStep, 0.0f, moveSpeed*pStep));



	//rotate the object in its own local space
	//_owner->rotate( glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f ) );

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
