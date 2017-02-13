#include "ActorMovement.h"
#include "AbstractActorBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ObjectActor.h"

#include <SFML/Window/Keyboard.hpp>

#include "mge/materials/ColorMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"
ActorMovement::ActorMovement(Mesh* pMesh,AbstractMaterial* pMaterial, float pSpeed) : AbstractActorBehaviour(), _moveSpeed(pSpeed), _mesh(pMesh), _material(pMaterial) {
	_timer = 0;
	_fireRate = 12;
	_fired = false;

	_coolDownTime = 50;
	_overheat = false;
	_heat = 0;
}

ActorMovement::~ActorMovement() {

}

//TODO: Decide whether to use applyforce or directly translate. 
void ActorMovement::update(float pStep) {
	float moveSpeed = 0.0f;
	btVector3 force;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeed = -_moveSpeed;
		force = btVector3(0,0,moveSpeed);
		_ownerBody->applyForce(force, force);		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeed = _moveSpeed;
		force = btVector3(0, 0, moveSpeed);
		_ownerBody->applyForce(force, force);		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moveSpeed = _moveSpeed;
		force = btVector3(moveSpeed, 0, 0);
		_ownerBody->applyForce(force, force);
		//_ownerBody->translate(force / 20);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moveSpeed = -_moveSpeed;
		force = btVector3(moveSpeed, 0, 0);
		_ownerBody->applyForce(force, force); 		
	}	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if(!_fired && !_overheat) {
			SpawnBullet();
			if (_heat > 100) {
				_overheat = true;				
			}
		}
		else {
			_timer++;
		}

		if (_timer >= _fireRate) {
			_fired = false;
			if (_timer >= _fireRate + _coolDownTime) {
				_overheat = false;
				_heat = 0;
			}			
		}
	} else {
		if (_heat >= 5)
			_heat -= 5;
	}
}

void ActorMovement::SpawnBullet()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -2.5f);

	ObjectActor* bullet = new ObjectActor(_owner->GetWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), 1, ObjectType::Bullet);
	//bullet->scale(glm::vec3(0.2f, 0.2f, 0.5f));
	bullet->setMesh(_mesh);
	bullet->setMaterial(_material);
	bullet->setActorBehaviour(new BulletBehaviour(1.0f, 1));		
	_owner->getParent()->add(bullet); //TODO: make sure we add it to the world, in case of nested objects.

	_timer = 0;
	_fired = true;
	_heat += 10;	
}

