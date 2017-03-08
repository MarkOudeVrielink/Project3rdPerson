#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"
#include "EnemyBehaviour.h"
#include "mgengine\Core\Actor.h"
#include "mygame/Waypoint.h"
#include "mygame/Enemy.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Behaviours\VanishBehaviour.h"

#include "mgengine\Materials\EnemyMaterial.h"
#include "mge\core\World.hpp"

#include "mgengine\Collision\CollisionFilters.h"

#include <list>
#include <glm.hpp>

EnemyBehaviour::EnemyBehaviour(std::vector<Waypoint*> *pWayPoints, glm::vec3  pMovingStep )
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
	
	if (!_levelEditorMode && !_kamikase) {
		AiBasic(pStep);
	}
	else if (!_levelEditorMode && _kamikase)
		AiKamikase(pStep);
	else
		UpdateEditorMode(pStep);

	/*	///TODO: Apply rotation with Slerp to smoothly look at the next waypoint while moving
		///May help http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
		//_owner->setTransform(glm::inverse(glm::lookAt(tarjet, tarjet + _owner->getWorldPosition(), glm::vec3(0, 1, 0))));
		glm::vec3 forward = glm::normalize( _owner->getLocalPosition() - tarjet);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::cross(forward, right);

		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1))
		);
		cout << _owner->getWorldPosition() << endl;
		*/

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
	
	glm::vec3 target(_tarjet->getWorldPos().x, 0, _tarjet->getWorldPos().z) ;

	target += _movingStep;
	
	glm::vec3 pos = _owner->getWorldPosition();
	if (_kamikase && !_kamikaseTargetSet)
	{
		_kamikaseTargetPos = _player->getWorldPosition();
		target = _kamikaseTargetPos+ _movingStep;
		_kamikaseTargetSet = true;
	}
	else if(_kamikase && _kamikaseTargetSet)
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
		_owner->SetRotation(glm::vec3(0,1,0), _angle);
	//}

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
	
	float length = glm::length(delta);
	if (length < _moveSpeed*pStep  && _wayPoints->size() > (float)_index) {
		_tarjet = _wayPoints->at(_index++);
		
	}
	else {
		if (_wayPoints->size() == _index &&length < _moveSpeed*pStep) //Increment index so we dont keep rotating in order to keep moving and leave the screen
			_index++;
		//if we dont have more waypoints, recalculate tarjet to keep moving "forward"
		
		delta = glm::normalize(delta);	//Get difference between object and target.
		_ownerBody->translate(btVector3(delta.x * _moveSpeed *pStep, 0.0f, delta.y * _moveSpeed*pStep));	 //Move toward target with set speed.
	}

	if (_wayPoints->size() >= _index && updateClock.getElapsedTime().asSeconds()- timeSinceLastShoot.asSeconds() > _shootRatio)//shoot
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
		cout << "haven´t started yet... returning" << endl;
		return;
		//_tarjet = _wayPoints->at(_index++);//index++ maybe
	}
	if (_movingBackwards == false)//Check if is first frame that we move backwards to invert waypoint
	{		
		//if ((float)_index > _wayPoints->size())
			//_index = _wayPoints->size();
		if (_index > 0 ) //check if there is a waypoint behind
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
	
	if (_index>=0) //check if there is a waypoint behind
	{//If we haven´t reached the last waypoint keep rotating
		_owner->SetRotation(glm::vec3(0, 1, 0), _angle);
	}

	glm::vec2 delta = glm::vec2(target.x, target.z) - glm::vec2(pos.x, pos.z);
	float length = glm::abs(glm::length(delta));

	if (length< _moveSpeed*pStep && _index >0)
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
	for (int i = 0; i < pAmount; i++) {
		glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -2.5f);

		ObjectActor* pickup = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_PickUp, CF::COL_PICKUP, CF::pickupCollidesWith);
		pickup->scale(glm::vec3(5.f, 5.f, 5.f));
		pickup->SetRotation(glm::vec3(1, 0, 0), 90);
		pickup->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::PickUp));
		pickup->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::PickUp));
		pickup->setActorBehaviour(new PickUpBehaviour(1, 10, 10));
		_owner->getWorld()->add(pickup);
	}

	//TODO: give drop a random start dir? differnt kind of drops?
}

void EnemyBehaviour::SpawnBullet()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, 2.5f);

	ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_ENEMYBULLET, CF::enemyBulletCollidesWith);
	bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Player));
	bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Player));
	bullet->setActorBehaviour(new BulletBehaviour(0.6f, 1.0f, 3.0f, Direction::Down, BulletOwner::Enemy));
	_owner->getParent()->add(bullet);
}

void EnemyBehaviour::SpawnExplosion()
{
	glm::vec3 spawnPoint = _owner->getWorldPosition();

	ObjectActor* explosion = new ObjectActor(_owner->getWorld(), "pickup", spawnPoint, new btSphereShape(2.0f), ActorType::Type_StaticObject, CF::COL_NOTHING, CF::pickupCollidesWith);	
	explosion->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Explosion));
	explosion->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Explosion));
	explosion->setActorBehaviour(new VanishBehaviour(2.0f));
	_owner->getWorld()->add(explosion);
}

void EnemyBehaviour::AiKamikase(float pStep)
{

}