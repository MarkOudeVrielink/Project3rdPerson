#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"
#include "EnemyBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mygame/Waypoint.h"
#include "mygame/Enemy.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Behaviours\VanishBehaviour.h"
#include "mgengine\Behaviours\PlayerBehaviour.h"

#include "mgengine\Materials\EnemyMaterial.h"
#include "mge\core\World.hpp"
#include <random>
#include "mgengine\Collision\CollisionFilters.h"
#include "mge\config.hpp"

#include <list>
#include <glm.hpp>

EnemyBehaviour::EnemyBehaviour(std::vector<Waypoint*> *pWayPoints, glm::vec3  pMovingStep)
{
	_movingStep = pMovingStep;
	_wayPoints = pWayPoints;
	_moveSpeed = 0.5f;
	_levelEditorMode = false;
}

EnemyBehaviour::EnemyBehaviour(std::vector<Waypoint*>* pWayPoints, float * ptime, glm::vec3  pMovingStep)
{
	_movingStep = pMovingStep;
	_wayPoints = pWayPoints;
	_moveSpeed = 0.5f;
	_levelEditorMode = true;//MoveAccording to the time, so no constant movement
	_referenceToArtificialLevelTime = ptime;
	_spawnedTime = *ptime;

}
//Constructor for kamikase
/*
EnemyBehaviour::EnemyBehaviour(GameObject * pPlayer,bool pLevelEditor = false, float * ptime = NULL)
{
	_player = pPlayer;
	_moveSpeed = 0.5f;
	_kamikase = true;
	_levelEditorMode = pLevelEditor;//MoveAccording to the time, so no constant movement
	if (ptime != NULL) {
		_referenceToArtificialLevelTime = ptime;
		_spawnedTime = *ptime;
	}
}
*/
EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::SaveOriginalTransform()
{
	_originalTransform = _owner->getTransform();
}



void EnemyBehaviour::update(float pStep)
{
	_animate();

	if (!_levelEditorMode && !_kamikase) {
		AiBasic(pStep);
	}
	else if (!_levelEditorMode && _kamikase)
		AiKamikase(pStep);
	else
		UpdateEditorMode(pStep);
	
}

void EnemyBehaviour::OnCollision(Actor * pOther)
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
				SpawnDrop();
				if (!_owner->getWorld()->getPlayerDead()) {
					Actor * Player = dynamic_cast<Actor*> (_owner->getWorld()->getMainPlayer());
					PlayerBehaviour * playerBehaviour = dynamic_cast<PlayerBehaviour*>(Player->getActorBehaviour());
					playerBehaviour->addScore(10.0f);
					cout << "Enemy Destroyed, Score Added: " << playerBehaviour->getScore() << endl;
				}
				owner->Destroy();
			}

		}
	}
}

void EnemyBehaviour::setup()
{
	_enemyMaterial = (EnemyMaterial*)_owner->getMaterial();
	_ownerBody->setCollisionFlags(_ownerBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void EnemyBehaviour::UpdateEditorMode(float pStep)
{
	if (*_referenceToArtificialLevelTime >= 0) {
		//If we havent move to the second we should be then calculate that position
		while (_secondsAlreadyRendered < *_referenceToArtificialLevelTime - _spawnedTime)//FORWARD
		{
			AiBasic(pStep);
			_secondsAlreadyRendered += 1.0f / 60.0f;//Harcoded guess that we have 60 frames per second

		}

		//If we havent move to the second we should be then calculate that position
		while (_secondsAlreadyRendered - *_referenceToArtificialLevelTime + _spawnedTime > 0.034f)//BACKWARDS when the difference is 1 sec
		{

			AiBasicBackWards(pStep);
			_secondsAlreadyRendered -= 1.0f / 60.0f;//Harcoded guess that we have 60 frames per second
		}
	}

}

#pragma region Getters and Setters
void EnemyBehaviour::setShootRatio(float pShootPerSec)
{
	_shootRatio = pShootPerSec;
}
void EnemyBehaviour::setSpeed(float pSpeed)
{
	_moveSpeed = pSpeed;
}
void EnemyBehaviour::setEnemyType(Materials::ID pType)
{
	_enemyType = pType;
}
float EnemyBehaviour::getShootRatio()
{
	return _shootRatio;
}
float EnemyBehaviour::getSpeed()
{
	return _moveSpeed;
}
Materials::ID EnemyBehaviour::getEnemyType()
{
	return _enemyType;
}
#pragma endregion

void EnemyBehaviour::_animate()
{
	_animationTime += _animtionClock.restart().asSeconds();

	switch (_enemyType)
	{
	case Materials::Yogurt:
		if (_animationTime < 0.5) {
			_owner->Slerp(glm::vec3(1, 0, 1), -1.40, 0.05);
		}
		else if (_animationTime < 1.0) {
			_owner->Slerp(glm::vec3(0, 0, 1), 0.52, 0.05);
		}
		else if (_animationTime > 1.5) {
			_animationTime = 0;
		}
		break;

	case Materials::Pizza:
		if (_animationTime < 0.5) {
			_owner->Slerp(glm::vec3(1, 0, 0), -0.78, 0.08);
		}
		else if (_animationTime < 1.5) {
			_owner->Slerp(glm::vec3(1, 0, 0), 0.78, 0.08);
		}
		else if (_animationTime > 2) {
			_animationTime = 0;
		}
		break;

	case Materials::Sushi:
		if (_animationTime < 0.2) {
			_owner->Slerp(glm::vec3(0, 1, 0), 0.5, 0.05);
		}
		else if (_animationTime <= 0.4) {
			_owner->Slerp(glm::vec3(1, 0, 0), -0.9, 0.05);
		}
		else if (_animationTime < 0.7) {
			_owner->Slerp(glm::vec3(0, 1, 0), -0.6, 0.05);
		}
		else if (_animationTime >= 0.7) {
			_animationTime = 0;
		}
		break;

	case Materials::Sandwich:
		if (_animationTime <= 0.3) {
			_owner->Slerp(glm::vec3(0, 1, 0), 1.1, 0.08);
			_ownerBody->translate(btVector3(0.4, 0, 0));
		}
		else if (_animationTime < 0.8) {
			_owner->Slerp(glm::vec3(0, 1, 0), 0, 0.08);
		}
		else if (_animationTime < 1.0) {
			_owner->Slerp(glm::vec3(0, 1, 0), -1.1, 0.08);
			_ownerBody->translate(btVector3(-0.4, 0, 0));
		}
		else if (_animationTime < 1.5) {
			_owner->Slerp(glm::vec3(0, 1, 0), 0, 0.08);			
		}
		else if (_animationTime > 1.6) {
			_animationTime = 0;
		}
		break;

	case Materials::Potato:
		_owner->setRotation(glm::vec3(0, 0, 1), _animationTime * 10); //Multiplier to speed up rotation.
		if (_animationTime >= 360)
			_animationTime = 0;
		break;
	default:
		break;
	}


}

//Pass the frame length and move the enemy the distance it should move in that frame
// Approx -> 0.01666666 length of frame * 60 = is equal to one sec
void EnemyBehaviour::AiBasic(float pStep)
{
	if (_tarjet == nullptr || _movingBackwards == true) {

		if ((float)_index <= 0)
			_index = 1;
		else _index++;

		if ((float)_index >= _wayPoints->size())
			_index = _wayPoints->size() - 1;
		//cout << _wayPoints->size() << endl;
		//ERROR, somehow size of waypoints is equal 0
		_tarjet = _wayPoints->at(_index);//index++ maybe
		_movingBackwards = false;
	}

	glm::vec3 target(_tarjet->getWorldPos().x, 0, _tarjet->getWorldPos().z);

	target += _movingStep;

	glm::vec3 pos = _owner->getWorldPosition();
	if (_kamikase && !_kamikaseTargetSet)
	{
		_kamikaseTargetPos = _player->getWorldPosition();
		target = _kamikaseTargetPos + _movingStep;
		_kamikaseTargetSet = true;
	}
	else if (_kamikase && _kamikaseTargetSet)
		target = _kamikaseTargetPos;
	if (_wayPoints->size() <= _index)
	{
		float x = -pos.x*1000.0f;
		float z = -pos.z*1000.0f;
		pos.x = x;
		pos.z = z;

	}
	btScalar dX = pos.x - target.x;
	btScalar dZ = pos.z - target.z;
	_angle = atan2(dX, dZ);

	//if (_wayPoints->size() >= (float)_index) {
	//_owner->setRotation(glm::vec3(0, 1, 0), _angle);
	//}

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);

	float length = glm::length(delta);

	if (length < _moveSpeed*pStep  && _wayPoints->size() >(float)_index) {
		_tarjet = _wayPoints->at(_index++);
	}

	else {
		if (_wayPoints->size() == _index &&length < _moveSpeed*pStep) //Increment index so we dont keep rotating in order to keep moving and leave the screen
			_index++;
		//if we dont have more waypoints, recalculate tarjet to keep moving "forward"

		delta = glm::normalize(delta);	//Get difference between object and target.
		_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.
	}

	if (_wayPoints->size() >= _index && updateClock.getElapsedTime().asSeconds() - timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
	{
		timeSinceLastShoot = updateClock.getElapsedTime();
		SpawnBullet();
	}
}

//The same AI but going to the previous waypoint
void EnemyBehaviour::AiBasicBackWards(float pStep)
{
	if (_tarjet == nullptr)
	{
		//cout << "haven´t started yet... returning" << endl;
		return;
		//_tarjet = _wayPoints->at(_index++);//index++ maybe
	}
	if (_movingBackwards == false)//Check if is first frame that we move backwards to invert waypoint
	{
		//if ((float)_index > _wayPoints->size())
			//_index = _wayPoints->size();
		if (_index > 0) //check if there is a waypoint behind
		{
			//cout << _wayPoints->at(_index--)->getPosition().x << endl;
			_index--;
			if (_index >= _wayPoints->size())
				_index = _wayPoints->size() - 1;
			_tarjet = _wayPoints->at(_index);
		}
		_movingBackwards = true;
	}
	//ROTATION
	glm::vec3 target(_tarjet->getWorldPos().x, 0, _tarjet->getWorldPos().z);
	target += _movingStep;
	glm::vec3 pos = _owner->getWorldPosition();

	btScalar dX = pos.x - target.x;
	btScalar dZ = pos.z - target.z;
	_angle = atan2(dX, dZ);

	if (_index >= 0) //check if there is a waypoint behind
	{//If we haven´t reached the last waypoint keep rotating
		_owner->Slerp(glm::vec3(0, 1, 0), _angle);
	}

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
	float length = glm::abs(glm::length(delta));

	if (length < _moveSpeed*pStep && _index >0)
	{
		_index--;

		_tarjet = _wayPoints->at(_index);
		cout << "Backwards" << endl;
	}
	else
	{
		delta = glm::normalize(delta);
		_ownerBody->translate(btVector3(delta.x * _moveSpeed*pStep, 0.0f, delta.y * _moveSpeed*pStep));
	}

}

void EnemyBehaviour::SpawnDrop(int pAmount)
{
	int random = rand() % (10 - 1) + 1;
	//if (random ==2 || random ==5)
		if (true) {
		for (int i = 0; i < pAmount; i++) {
			glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -2.5f);

			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<int> uni(1, 7); // guaranteed unbiased

			auto random_integer = uni(rng);
			int randomType = rand() % (10 - 1) + 1;
			ObjectActor* pickup = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_PickUp, CF::COL_PICKUP, CF::pickupCollidesWith);
			
			pickup->Slerp(glm::vec3(1, 0, 0), 90);
			pickup->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::ID(20+ random_integer)));
			pickup->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::ID(20 + random_integer)));
			pickup->scale(glm::vec3(1, 1, 1)*getScale(20 + random_integer));
			pickup->setActorBehaviour(new PickUpBehaviour(1, 10, 10));
			_owner->getWorld()->add(pickup);
		}
	}

	//TODO: give drop a random start dir? differnt kind of drops?
}

void EnemyBehaviour::SpawnBullet()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, 2.5f);

	ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
	//bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::ID(getBulletTypeBasedOnEnemy(_enemyType))));
	bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::ID(getBulletTypeBasedOnEnemy(_enemyType))));
	bullet->setActorBehaviour(new BulletBehaviour(0.6f, 1.0f, 3.0f, Direction::Down, BulletOwner::Enemy));
	glm::vec3 scaleBullet(1, 1, 1);
	scaleBullet *= getScale(_enemyType);
	bullet->scale(scaleBullet);
	_owner->getParent()->add(bullet);
}

int EnemyBehaviour::getBulletTypeBasedOnEnemy(int pIndex)
{
	return  pIndex + 7;/*
	switch (pIndex)
	{
	case 2://Yogurt

		break;
	case 3://Sushi
		break;
	case 4://Sandwich
		break;
	case 5://Potato
		break;
	case 6://Pizza
		break;
	case 7://Muffin
		break;
	case 8://Boss
		break;

	}*/
}

float EnemyBehaviour::getScale(int pIndex)
{
	
	switch (pIndex)
	{
	case 2://Yogurt
		return 10;
		break;
	case 3://Sushi
		return 1;
		break;
	case 4://Sandwich
		return 1;
		break;
	case 5://Potato
		return 10;
		break;
	case 6://Pizza
		return 2;
		break;
	case 7://Muffin
		return 5;
		break;
	case 8://Boss
		return 1;
		break;
	case 21:
		return 5;
		break;
	case 22:
		return 1;
		break;
	case 23:
		return 1;
		break;
	case 24:
		return 1;
		break;
	case 25:
		return 1;
		break;
	case 26:
		return 1;
		break;
	case 27:
		return 1;
		break;
	}
}

void EnemyBehaviour::SpawnExplosion()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition();
	
	ObjectActor* explosion = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_StaticObject, CF::COL_NOTHING, CF::pickupCollidesWith);
	//explosion->setActorBehaviour(new VanishBehaviour(2.0f));
	//ObjectActor* explosion = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_StaticObject, CF::COL_NOTHING, CF::pickupCollidesWith);		
	explosion->setActorBehaviour(new VanishBehaviour(config::MGE_TEXTURE_PATH + "mini_explosion.png"));

	_owner->getWorld()->add(explosion);
}

void EnemyBehaviour::AiKamikase(float pStep)
{

}