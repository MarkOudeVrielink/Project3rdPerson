#include "mgengine\Behaviours\VanishBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mge\core\World.hpp"


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
	_animation = new Animation(_owner->getWorld()->getRenderWindow(), _texture, sf::Vector2i(149, 158), 7.0f, 0.5f);
	_animation->setRepeating(false);

	glm::vec3 position = _owner->getWorldPosition();
	sf::Vector2f spritePosition = sf::Vector2f(position.x * 4, position.z * 13);
	_animation->setPosition(spritePosition);
	//_animation->setScale(sf::Vector2f(0.1f, 0.1f));
}

