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
	//_bossState = BossState::RotateShoot;
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

	/*if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)//shoot
	{
		timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
		SpawnBullet();
	}*/
	switch (_bossState) {
	case 0: //Idle
		delay = 2;
		_owner->SetRotation(glm::vec3(0, 1, 0), 0);
		if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)
		{
			timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
			int random = rand() %( 5 - 1) + 1;
			cout << "random Number generated" << random << endl;
			_bossState = BossState(random);
			//_bossState = BossState::Angry;

		}
		break;
	case 1: //Rotate and shoot
		delay = 6;
		if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)
		{
			_owner->SetRotation(glm::vec3(0, 1, 0), 0);
			timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
			_bossState = BossState::Idle;
		}
		RotateAndshoot(pStep);
		break;
	case 2:	//Spawn kamikase
		_owner->SetRotation(glm::vec3(0, 1, 0), 0);
		delay = 5;
		if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)
		{
			_owner->SetRotation(glm::vec3(0, 1, 0), 0);
			timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
			_bossState = BossState::Idle;
		}
		SpawnEnemiesKamikase(pStep);
		break;
	case 3: //Charge to player
	//	_owner->SetRotation(glm::vec3(0, 1, 0), 0);
		if (!chargedAgainstPlayer)
			ChargePlayer(pStep);
		else if (chargedAgainstPlayer &&GoToSpawnPosition(pStep))
		{
			_owner->SetRotation(glm::vec3(0, 1, 0), 0);
			_moveSpeed = 60;
			_bossState = BossState::Idle;
			chargedAgainstPlayer = false;
		}
		break;
	case 4:	 //20% health Bounce through screen while rotating and shooting, maybe also spawning enemies 
		delay = 6;
		_owner->SetRotation(glm::vec3(0, 1, 0), 0);
		if(!AiBasicDone)
		AiBasicDone = AiBasic(pStep);
		else if (AiBasicDone && GoToSpawnPosition(pStep))
		{
			_moveSpeed = 60;
			_bossState = BossState::Idle;
			AiBasicDone = false;
		}
		break;
	}


}
void BossBehaviour::RotateAndshoot(float pStep)
{
	_angle2 += pStep * 5;
	//_angle2 = 0;
	_owner->SetRotation(glm::vec3(0, 1, 0), _angle2);

	if (shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
	{
		timeSinceLastShoot = shootClock.getElapsedTime();
		SpawnBullet(1,glm::vec3(0,0,1),_angle2);
		//SpawnBullet();
	}
}
//Hardcoded properties
void BossBehaviour::SpawnEnemiesKamikase(float pStep)
{
	if (shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > 1)//spawn
	{
		_wayPointsKamikase = *_wayPoints;
		timeSinceLastShoot = shootClock.getElapsedTime();

		glm::vec3 spawnPoint = _owner->getWorldPosition();// +directions.at(i)*.25f;//radius(?)
		ControlledActor* Enemy1 = new ControlledActor(_world, "Enemy", spawnPoint, new btSphereShape(5), ActorType::Type_Enemy, 1, CF::COL_ENEMY, CF::enemyCollidesWith);
		Waypoint * SpawnWaypoint = new Waypoint(spawnPoint, sf::Vector2f(500, 500), 1, 580, _wayPoints->at(0)->getRenderWindow());
		glm::vec3 posplayerglmvec3;
		if(!_world->getPlayerDead())
		 posplayerglmvec3 = _world->getMainPlayer()->getWorldPosition();
		else
		 posplayerglmvec3 = glm::vec3(0,0,-10000);
		Waypoint * playerPos = new Waypoint(glm::vec3(posplayerglmvec3.x, posplayerglmvec3.y, posplayerglmvec3.z), sf::Vector2f(500, 500), 1, 580, SpawnWaypoint->getRenderWindow());

		cout << "ENEMY Kamikase CREATED" << endl;
		_wayPointsKamikase.clear();
		_wayPointsKamikase.push_back(SpawnWaypoint);
		_wayPointsKamikase.push_back(playerPos);
		Enemy1->setMesh(_world->GetResourceManager()->getMesh(Meshes::Potato));
		Enemy1->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Potato));
		EnemyBehaviour* behave = new EnemyBehaviour(&_wayPointsKamikase, _movingStep);
		behave->setSpeed(50);
		behave->setEnemyType(Materials::Potato);
		behave->setShootRatio(100);
		behave->setEnemyType(_enemyType);
		Enemy1->SetHealth(1);
		Enemy1->setActorBehaviour(behave);

		_owner->getParent()->add(Enemy1);
	}

}
bool BossBehaviour::ChargePlayer(float pStep)
{
	int delay = 2;
	//Rotate for 2 seconds while spawning enemies and then charge to player
	if (behaviourClock.getElapsedTime().asSeconds() - timeSinceLastBehaviourChange.asSeconds() > delay)
	{
		//save player and boss original posisions including the direction
		if (!_playerDirectionSaved && !_world->getPlayerDead()) {
			_directionToPlayer = _world->getMainPlayer()->getWorldPosition();			
			_playerDirectionSaved = true;
			_owner->SetRotation(glm::vec3(0, 1, 0), 0);
			_moveSpeed = 250;
			glm::vec3 pos = _owner->getWorldPosition();
			float dX = pos.x - _directionToPlayer.x;
			float dZ = pos.z - _directionToPlayer.z;
			_angle = atan2(dX, dZ);
			_owner->SetRotation(glm::vec3(0, 1, 0), _angle);

			pos = _owner->getWorldPosition();
			delta.x = _directionToPlayer.x - pos.x;
			delta.y = _directionToPlayer.z - pos.z;
		}
		glm::vec3 pos = _owner->getWorldPosition();
		glm::vec2 delta2;
		delta2.x = _directionToPlayer.x - pos.x;
		delta2.y = _directionToPlayer.z - pos.z;
		float length = glm::length(delta2); //length from boss to player

		if (length < _moveSpeed*pStep)//we reached our destiny but we want to keep moving 
			reachedPlayer = true;
		//if we reached our destiny keep moving for 2 seconds
		if ((reachedPlayer) && (shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > 2)) {//borrow shooting timers			
			timeSinceLastShoot = shootClock.getElapsedTime();
			_playerDirectionSaved = false;
			timeSinceLastBehaviourChange = behaviourClock.getElapsedTime();
			chargedAgainstPlayer = true;
			reachedPlayer = false;
			return true;
		}
		else {///move
			delta = glm::normalize(delta);
			_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.

		}
		return false;
	}
	else
	{
		SpawnEnemiesKamikase(pStep);
		_angle2 -= pStep * 30;
		_owner->SetRotation(glm::vec3(0, 1, 0), _angle2);
	}
	return false;
}
bool BossBehaviour::GoToSpawnPosition(float pStep)
{

	_moveSpeed = 2.5f;
	glm::vec3 pos = _owner->getWorldPosition();
	float dX = pos.x - _originalSpawn.x;
	float dZ = pos.z - _originalSpawn.z;
	_angle = atan2(dX, dZ);
	_owner->SetRotation(glm::vec3(0, 1, 0), _angle);
	glm::vec2 delta = glm::vec2(_originalSpawn.x, _originalSpawn.z) - glm::vec2(pos.x, pos.z);
	_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.
	float length = glm::length(delta);
	if (length < _moveSpeed*pStep) {

		return true;
	}
	return false;

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
				owner->getWorld()->setBossDeath(true);
				owner->Destroy();
			}

		}
	}
}

void BossBehaviour::setup()
{
	_enemyMaterial = (EnemyMaterial*)_owner->getMaterial();
	_ownerBody->setCollisionFlags(_ownerBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	_originalSpawn = _owner->getWorldPosition();
	_owner->getWorld()->GetResourceManager()->PlayMusic(Music::BossTheme);
}

void BossBehaviour::SpawnBullet()
{

	vector<glm::vec3> directions = { glm::vec3(0, 0, 1) ,glm::vec3(0, 0, -1) ,glm::vec3(-1, 0, 0) ,glm::vec3(1, 0, 0) };
	for (int i = 0; i < 4;i++)
	{
		glm::vec3 spawnPoint = _owner->getWorldPosition() + directions.at(i)*.25f;//radius(?)
		ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
		BulletBehaviour * bulletBehave0 = new BulletBehaviour(0.6f, 1.0f, 3.0f, Direction::Custom, BulletOwner::Enemy);
		bullet->scale(glm::vec3(10, 10, 10));
		bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Bullet));
		bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::BulletBoss));
		bullet->setActorBehaviour(bulletBehave0);
		bulletBehave0->setBulletDirection(glm::rotate(directions.at(i), _angle2, glm::vec3(0, 1, 0)));//Down
		_owner->getParent()->add(bullet);
	}

}
void BossBehaviour::SpawnBullet(float pBulletPower, glm::vec3 pDirection, float pAngle)
{
	
	vector<glm::vec3> directions = { glm::vec3(0, 0, 1) ,glm::vec3(0, 0, -1) ,glm::vec3(-1, 0, 0) ,glm::vec3(1, 0, 0) };
	for (int i = 0; i < 4;i++)
	{
		glm::vec3 spawnPoint = _owner->getWorldPosition() + directions.at(i)*.25f;//radius(?)
		ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
		BulletBehaviour * bulletBehave0 = new BulletBehaviour(0.6f, 1.0f, 3.0f, Direction::Custom, BulletOwner::Enemy);
		bullet->scale(glm::vec3(10, 10, 10));
		bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Bullet));
		bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::BulletBoss));
		bullet->setActorBehaviour(bulletBehave0);
		bulletBehave0->setBulletDirection(glm::rotate(directions.at(i), _angle2, glm::vec3(0, 1, 0)));//Down
		float angleBullet = 0;
		switch (i)
		{
		case 0:
			angleBullet = glm::radians(180.0f);
			break;
		case 1:
			angleBullet = 0;
			break;
		case 2:
			angleBullet = glm::radians(-90.0f);
			break;
		case 3:
			angleBullet = glm::radians(90.0f);
			break;
		}
		bulletBehave0->setBulletRotation(glm::vec3(0, 1, 0), (pAngle)+angleBullet);
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

bool BossBehaviour::AiBasic(float pStep)
{
	_moveSpeed = 50;
	if (_tarjet == nullptr) {

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

	
	_owner->SetRotation(glm::vec3(0, 1, 0), _angle2);
	

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
	float length = glm::length(delta);

	if (length < _moveSpeed*pStep  && _wayPoints->size() >(float)_index) {
		_tarjet = _wayPoints->at(_index++);
	}
	else {
		if (_wayPoints->size() == _index &&length < _moveSpeed*pStep) { //Increment index so we dont keep rotating in order to keep moving and leave the screen
			_index = 0;
			return true;
		}
		//if we dont have more waypoints, recalculate tarjet to keep moving "forward"
		
		delta = glm::normalize(delta);	//Get difference between object and target.

		_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.
	}
//	RotateAndshoot(pStep);

	
	if (_wayPoints->size() >= _index && shootClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
	{
		timeSinceLastShoot = shootClock.getElapsedTime();
		SpawnBullet(1, glm::vec3(1, 1, 1), _angle2);
	}
	return false;
}