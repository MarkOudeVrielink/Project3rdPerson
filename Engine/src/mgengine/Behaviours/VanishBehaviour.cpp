#include "mgengine\Behaviours\VanishBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mge\core\World.hpp"
#include "mge\core\Camera.hpp"

#include <math.h>

VanishBehaviour::VanishBehaviour(std::string pFileName)
{
	if (!_texture.loadFromFile(pFileName)) {
		printf("COULD NOT LOAD SPRITE %s", pFileName);
	}
}

VanishBehaviour::~VanishBehaviour()
{
	delete _animation;
}

void VanishBehaviour::update(float pStep)
{
	_animation->update(pStep);

	if (_animation->isFinished()) {
		_owner->Destroy();
	}
}

void VanishBehaviour::OnCollision()
{
}

void VanishBehaviour::setup()
{
	_animation = new Animation(_owner->getWorld()->getRenderWindow(), _texture, sf::Vector2i(149, 158), 7.0f, 0.3f);
	_animation->setRepeating(false);	
	
	sf::Vector2f spritePosition = sf::Vector2f(_ScreenSpaceCoord());
	_animation->setPosition(spritePosition);	
}

/*Get the screen coördinate of the owner.*/
sf::Vector2f VanishBehaviour::_ScreenSpaceCoord()
{
	Camera* camera = _owner->getWorld()->getMainCamera();

	//Calculate the ViewProjection matrix.
	glm::mat4 projectionView = camera->getProjection() * glm::inverse(camera->getWorldTransform()); 

	//Move the position to normalized device space.
	glm::vec4 position = glm::normalize(projectionView * glm::vec4(_owner->getWorldPosition(), 1));	
	sf::Vector2u window = sf::Vector2u(_owner->getWorld()->getRenderWindow()->getSize());
	
	//Get our position between [0 , 1] and multiply it with the total screen size to get the screen coördinates.
	float winX = (float)round(((position.x + 1) / 2.0) * window.x);
	float winY = (float)round(((1 - position.y) / 2.0) * window.y);

	return sf::Vector2f(winX, winY);
}

