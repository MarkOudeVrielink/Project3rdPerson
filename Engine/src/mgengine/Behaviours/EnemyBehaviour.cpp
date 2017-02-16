#include "mgengine\Behaviours\EnemyBehaviour.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "AbstractActorBehaviour.h"

#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ObjectActor.h"

#include "mge\materials\AbstractMaterial.hpp"
#include "mge/core/Mesh.hpp"

#include "mgengine\Collision\CollisionFilters.h"

#include <SFML/Window/Keyboard.hpp>//UNDO: unnecessary include, is here just for testing purposes.

EnemyBehaviour::EnemyBehaviour(Mesh* pDropMesh, AbstractMaterial* pDropMaterial) 
	: _dropMesh(pDropMesh), _dropMaterial(pDropMaterial)
{
}

EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::update(float pStep)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		SpawnBullet();
	}
}

void EnemyBehaviour::OnCollision(Actor * pOther)
{
}

void EnemyBehaviour::SpawnDrop(int pAmount)
{
	for (int i = 0; i < pAmount; i++) {
		glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -2.5f);

		ObjectActor* pickup = new ObjectActor(_owner->GetWorld(), "pickup", spawnPoint, new btSphereShape(0.7f), ActorType::Type_PickUp, CF::COL_PICKUP, CF::pickupCollidesWith);
		//bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
		pickup->setMesh(_dropMesh);
		pickup->setMaterial(_dropMaterial);
		pickup->setActorBehaviour(new PickUpBehaviour());
		_owner->getParent()->add(pickup);
	}
	//HACK: make sure we add it to the world, in case of nested objects.
	//TODO: give drop a random start dir?
}

void EnemyBehaviour::SpawnBullet()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, 2.5f);

	ObjectActor* bullet = new ObjectActor(_owner->GetWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
	bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	bullet->setMesh(_dropMesh);
	bullet->setMaterial(_dropMaterial);
	bullet->setActorBehaviour(new BulletBehaviour(0.6f, 1.0f, 50.0f , Direction::Down, BulletOwner::Enemy));
	_owner->getParent()->add(bullet);
}
