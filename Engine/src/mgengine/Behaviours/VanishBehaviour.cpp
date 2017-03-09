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
	//_animation = new Animation(_owner->getWorld()->getRenderWindow(), _texture, sf::Vector2i(661, 377), 15.0f, 0.5f);
	//_animation->setScale(sf::Vector2f(3.0f, 3.0f));
	_animation->setRepeating(false);

	Camera* camera = _owner->getWorld()->getMainCamera();
	glm::mat4 projectionView = camera->getProjection() * glm::inverse(camera->getWorldTransform());

	glm::vec4 position = glm::vec4(_owner->getWorldPosition(), 1) * projectionView;
	//glm::vec3 position =_owner->getWorldPosition();
	//sf::Vector2i window= sf::Vector2i(_owner->getWorld()->getRenderWindow()->getSize());

	printf("pos x: %f | pos y: %f | pos z: %f\n", position.x, position.y, position.z);
	int winX = (int)round(((position.x + 1) / 2.0) * 1920 );
	int winY = (int)round(((1 - position.z) / 2.0) * 1080);

	printf("x: %d | z: %d\n", winX, winY);
	//glm::vec3 position =_owner->getWorldPosition();
	sf::Vector2f spritePosition = sf::Vector2f((float)winX, (float)winY);
	_animation->setPosition(spritePosition);
	//_animation->setScale(sf::Vector2f(0.1f, 0.1f));
}

