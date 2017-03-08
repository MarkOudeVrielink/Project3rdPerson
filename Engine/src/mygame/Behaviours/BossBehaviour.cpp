#include "BossBehaviour.h"
#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"
#include "EnemyBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mygame/Waypoint.h"
#include "mygame/Enemy.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Behaviours\VanishBehaviour.h"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/config.hpp"

#include "mgengine\Materials\EnemyMaterial.h"
#include "mge\core\World.hpp"

#include "mgengine\Collision\CollisionFilters.h"

#include <list>
#include <glm.hpp>

BossBehaviour::BossBehaviour(std::vector<Waypoint*>* pWayPoints, glm::vec3 pMovingStep, World* pWorld)
{
	_movingStep = pMovingStep;
	_wayPoints = pWayPoints;
	_moveSpeed = 0.5f;
	_world = pWorld;
}

void BossBehaviour::update(float pStep)
{
	_bossState = BossState::RotateShoot;
	//RotateAndshoot(pStep);
	/*
	Idle 3 sec
	Shoot 6 sec
	Idle 1
	Kamikase 5 sec
	Idle 1
	shoot 6 sec
	Idle 1
	Bounce 1 lap
	charge 

	-20% Health: 
	Bounce 2 laps
	Speed *= 1.5
	shootRate  *= 1.5;
	*/
	int delay = 3;
	BossState nextBehaviour =BossState::RotateShoot;
	/*if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)//shoot
	{
		timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
		SpawnBullet();
	}*/
	switch (_bossState) {
	case 0: //Idle

		break;
	case 1: //Rotate and shoot
		RotateAndshoot(pStep);
		break;
	case 2:	//Spawn kamikase
		SpawnEnemiesKamikase(pStep);
		break;
	case 3: //Charge to player
		break;
	case 4:	 //20% health Bounce through screen while rotating and shooting, maybe also spawning enemies 
		break;
	}
	
	
}
void BossBehaviour::RotateAndshoot(float pStep)
{
	_angle2 += pStep * 5;
	_owner->SetRotation(glm::vec3(0, 1, 0), _angle2);

	if (shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
	{
		timeSinceLastShoot = shootClock.getElapsedTime();
		SpawnBullet();
	}
}
//Hardcoded properties
void BossBehaviour::SpawnEnemiesKamikase(float pStep)
{
	if (shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > 1)//shoot
	{
		timeSinceLastShoot = shootClock.getElapsedTime();

		glm::vec3 spawnPoint = _owner->getWorldPosition();// +directions.at(i)*.25f;//radius(?)
		ControlledActor* Enemy1 = new ControlledActor(_world, "Enemy", spawnPoint, new btSphereShape(5), ActorType::Type_Enemy, 1, CF::COL_ENEMY, CF::enemyCollidesWith);
		Waypoint * SpawnWaypoint = new Waypoint(spawnPoint, sf::Vector2f(500, 500), 1, 580, _wayPoints->at(0)->getRenderWindow());
		glm::vec3 posplayerglmvec3 = _world->getMainPlayer()->getWorldPosition();
		Waypoint * playerPos = new Waypoint(glm::vec3(posplayerglmvec3.x, posplayerglmvec3.y, posplayerglmvec3.z), sf::Vector2f(500, 500), 1, 580, SpawnWaypoint->getRenderWindow());

		cout << "ENEMY Kamikase CREATED" << endl;
		AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png"));
		_wayPoints->clear();
		_wayPoints->push_back(SpawnWaypoint);
		_wayPoints->push_back(playerPos);
		Enemy1->setMesh(_world->GetResourceManager()->getMesh(Meshes::Pizza));
		Enemy1->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Pizza));
		EnemyBehaviour* behave = new EnemyBehaviour(_wayPoints, _movingStep);
		behave->setSpeed(50);
		behave->setShootRatio(1);
		behave->setEnemyType(_enemyType);
		Enemy1->SetHealth(1);
		Enemy1->setActorBehaviour(behave);

		_owner->getParent()->add(Enemy1);
	}

}
void BossBehaviour::ChargePlayer(float pStep)
{
	_owner->getParent()->getParent();
}
void BossBehaviour::BounceInScreen(float pStep)
{
}
void BossBehaviour::OnCollision(Actor * pOther)
{
	ActorType type = pOther->getType();

	if (type == ActorType::Type_Bullet || type == ActorType::Type_Nova) {
		BulletBehaviour* bullet = (BulletBehaviour*)pOther->getActorBehaviour();
		if (bullet->getOwner() == BulletOwner::Player) {
			ControlledActor* owner = (ControlledActor*)_owner;
			owner->TakeDamage(bullet->getPower());

			//_enemyMaterial->setDamaged(true);
			_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Enemy_Hit);

			if (owner->GetHealth() <= 0) {
				SpawnExplosion();
				

				owner->Destroy();
			}

		}
	}
}

void BossBehaviour::setup()
{
	_enemyMaterial = (EnemyMaterial*)_owner->getMaterial();
}

void BossBehaviour::SpawnBullet()
{
	vector<glm::vec3> directions = { glm::vec3(0, 0, 1) ,glm::vec3(0, 0, -1) ,glm::vec3(-1, 0, 0) ,glm::vec3(1, 0, 0) };
	for (int i = 0; i < 4;i++)
	{
		glm::vec3 spawnPoint = _owner->getWorldPosition() + directions.at(i)*.25f;//radius(?)
		ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
		BulletBehaviour * bulletBehave0 = new BulletBehaviour(0.6f, 1.0f, 3.0f, Direction::Custom, BulletOwner::Enemy);
		bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
		bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Player));
		bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Player));
		bullet->setActorBehaviour(bulletBehave0);
		bulletBehave0->setBulletDirection(glm::rotate(directions.at(i), _angle2,glm::vec3(0,1,0)));//Down
		_owner->getParent()->add(bullet);
	}
}

void BossBehaviour::SpawnExplosion()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition();

	ObjectActor* explosion = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_StaticObject, CF::COL_NOTHING, CF::pickupCollidesWith);
	explosion->setActorBehaviour(new VanishBehaviour(config::MGE_TEXTURE_PATH + "mini_explosion.png"));
	_owner->getWorld()->add(explosion);
}

void BossBehaviour::setShootRatio(float pShootPerSec)
{
	_shootRatio = pShootPerSec;
}

void BossBehaviour::setSpeed(float pSpeed)
{
	_moveSpeed = pSpeed;
}

void BossBehaviour::setEnemyType(Materials::ID pType)
{
	_enemyType = pType;
}

float BossBehaviour::getShootRatio()
{
	return _shootRatio;
}

float BossBehaviour::getSpeed()
{
	return _moveSpeed;
}

Materials::ID BossBehaviour::getEnemyType()
{
	return _enemyType;
}

void BossBehaviour::AiBasic(float pStep)
{
	if (_tarjet == nullptr ) {

		if ((float)_index <= 0)
			_index = 1;
		else _index++;

		if ((float)_index >= _wayPoints->size())
			_index = _wayPoints->size() - 1;
		_tarjet = _wayPoints->at(_index);//index++ maybe
	}

	glm::vec3 target(_tarjet->getWorldPos().x, 0, _tarjet->getWorldPos().z);
	target += _movingStep;
	glm::vec3 pos = _owner->getWorldPosition();	
	btScalar dX = pos.x - target.x;
	btScalar dZ = pos.z - target.z;
	//_angle2 = atan2(dX, dZ);

	if (_wayPoints->size() >= (float)_index) {
	//	_owner->SetRotation(glm::vec3(0, 1, 0), _angle2);
	}

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
	float length = glm::length(delta);

	if (length < _moveSpeed*pStep  && _wayPoints->size() >(float)_index) {
		_tarjet = _wayPoints->at(_index++);
	}
	else {
		if (_wayPoints->size() == _index &&length < _moveSpeed*pStep) //Increment index so we dont keep rotating in order to keep moving and leave the screen
			_index++;
		//if we dont have more waypoints, recalculate tarjet to keep moving "forward"
		if (_wayPoints->size() < _index)
		{
			target *= 100;
			delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
		}
		delta = glm::normalize(delta);	//Get difference between object and target.
		_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.
	}

	if (_wayPoints->size() >= _index && shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
	{
		timeSinceLastShoot = shootClock.getElapsedTime();
		SpawnBullet();
	}
}