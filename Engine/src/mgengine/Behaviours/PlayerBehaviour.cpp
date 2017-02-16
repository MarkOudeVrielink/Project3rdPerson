#include "PlayerBehaviour.h"
#include "AbstractActorBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"

#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"
#include <SFML/Window/Keyboard.hpp>

#include "mge/materials/ColorMaterial.hpp"
#include "mge/core/Mesh.hpp"

#include "mgengine\Collision\CollisionFilters.h"

PlayerBehaviour::PlayerBehaviour(Mesh* pMesh,AbstractMaterial* pMaterial, float pSpeed) : AbstractActorBehaviour(), _moveSpeed(pSpeed), _mesh(pMesh), _material(pMaterial) {
	_timer		= 0;
	_fireRate	= 12;
	_fired		= false;

	_coolDownTime	= 50;
	_overheat		= false;
	_heat			= 0;
	
	_charge			= 100;
	_maxCharge		= 100;
	_beginNova		= 0;
	_endNova		= 30;

	_score			= 0;
}

PlayerBehaviour::~PlayerBehaviour() {

}

//TODO: Decide whether to use applyforce or directly translate. 
void PlayerBehaviour::update(float pStep) {
	float moveSpeed = 0.0f;
	btVector3 force = btVector3(0,0,0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeed = -_moveSpeed;
		force = btVector3(0,0,moveSpeed);
		//_ownerBody->applyForce(force, force);
		_ownerBody->translate(force / 50);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeed = _moveSpeed;
		force = btVector3(0, 0, moveSpeed);
		//_ownerBody->applyForce(force, force);
		_ownerBody->translate(force / 50);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moveSpeed = _moveSpeed;
		force = btVector3(moveSpeed, 0, 0);
		//_ownerBody->applyForce(force, force);
		_ownerBody->translate(force / 50);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moveSpeed = -_moveSpeed;
		force = btVector3(moveSpeed, 0, 0);
		//_ownerBody->applyForce(force, force); 
		_ownerBody->translate(force / 50);

	}	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if(!_fired && !_overheat) {
			SpawnBullet(1.0f);
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && _charge == _maxCharge) {
		SpawnNova();
		_charge = 0;
	}
}

void PlayerBehaviour::OnCollision(Actor * pOther)
{
	ActorType type = pOther->GetType();

	if(type == ActorType::Type_Enemy){
		_ownerBody->translate(btVector3(0,0,5));

		ControlledActor* player = (ControlledActor*)_owner;
		player->TakeDamage(4);
	}
	else if (type ==ActorType::Type_PickUp) {
		PickUpBehaviour* pickup = (PickUpBehaviour*)pOther->getActorBehaviour();
		_charge += pickup->GetCharge();
		_score	+= pickup->GetPoints();
		std::cout << "score: " << _score << " | charge " << _charge << std::endl;

		_ownerBody->setLinearVelocity(btVector3(0, 0, 0));
	}
}

/*works in a crummy way... but it serves it's purpose.*/
void PlayerBehaviour::SpawnNova()
{	
	glm::vec3 spawnPoint = glm::vec3(0, 0, 20);

	ObjectActor* nova = new ObjectActor(_owner->GetWorld(), "Nova", spawnPoint, new btBoxShape(btVector3(75,1,1)), ActorType::Type_Nova, CF::COL_PLAYERNOVA, CF::playerNovaCollidesWith, 1);
	nova->setActorBehaviour(new BulletBehaviour(5.5f, 10, 10));
	nova->setMesh(_mesh);
	nova->setMaterial(_material);
	_owner->getParent()->add(nova);

	std::cout << "Nova" << std::endl;
}

//TODO: Maybe make some way off accessing the world cache with textures and meshes? Just so that I don't have to pass those on in the contructor.
void PlayerBehaviour::SpawnBullet(float pBulletPower)
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -2.5f);

	ObjectActor* bullet = new ObjectActor(_owner->GetWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_PLAYERBULLET, CF::playerBulletCollidesWith, 1);
	bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	bullet->setMesh(_mesh);
	bullet->setMaterial(_material);
	bullet->setActorBehaviour(new BulletBehaviour(0.6f, pBulletPower));		
	_owner->getParent()->add(bullet); //HACK: make sure we add it to the world, in case of nested objects.

	_timer = 0;
	_fired = true;
	_heat += 10;	
}

