#include "EnemyWave.h"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/config.hpp"
#include "mygame\Behaviours\EnemyBehaviour.h"
#include "mgengine\Core\ControlledActor.h"
#include "mygame\Behaviours\BossBehaviour.h"
#include "mgengine\Collision\CollisionFilters.h"

EnemyWave::EnemyWave()
{
	for (int i = 0; i < 5;i++)
	{
		Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
		_enemies.push_back(Enemy1);
	}	
}
//TODO: Set the delay between enemies relative to the speed of the enemies to avoid them colliding between them

EnemyWave::~EnemyWave()
{
	
}
//Add waypoint to the list of waypoints in this wave
void EnemyWave::addWaypoint(Waypoint * pWaypoint, float pSec)
{
	//Set when the wave willl appear based on the first waypoint
	if (_wayPoints.empty())
	{
		_startTimeWave = pSec;
		//cout << "Waypoints Empty.... Adding waypoint" << _startTimeWave << endl;

	}
	//else
		//cout << "NOT EMPTY HUEUHUE" << endl;
	_wayPoints.push_back(pWaypoint);
}
void EnemyWave::addMainWaypointDirection(Waypoint * pWaypoint, float pSec)
{
	_mainWaypointDirection = pWaypoint;
}
Waypoint* EnemyWave::GetMainWaypointDirection()
{
	return _mainWaypointDirection;
}
//Return reference of the list of all the waypoints
const std::vector<Waypoint*>* EnemyWave::getWaypoints() const
{
	return &_wayPoints;
}
//Loop and draw all the waypoints in screen
void EnemyWave::DrawWaypoints()
{//TODO: Recalculate
	glm::vec2 prevWaypointPos = glm::vec2(0, 0);
	//cout << "DRAW WAYPOINT" << endl;
	float lengthTraveled =0;
	//if(_startTimeWave + (_delayBetweenEnemies*_sizeWave)>*_snapTime)
	//Add getWorldPos to the waypoints
	lengthTraveled = (*_snapTime*_speed- ((_startTimeWave + (_delayBetweenEnemies*_sizeWave))*_speed));
	//cout << lengthTraveled << endl;
	float lenghtbetweenWaypoints = 0;
	//we are going backwards 
	/*
	for (int i = _wayPoints.size()-1;i>=0;i--)
	{
		glm::vec2 delta = glm::vec2(_wayPoints.at(i)->getPosition().x, _wayPoints.at(i)->getPosition().y) - prevWaypointPos;
		prevWaypointPos += glm::vec2(_wayPoints.at(i)->getPosition().x, _wayPoints.at(i)->getPosition().y);
		if((glm::length(delta)) >lengthTraveled)
			_wayPoints.at(i)->Draw();
	}
	*/
	if(_mainWave)
		for (auto& waypoint : _wayPoints)
		{
			waypoint->Draw();
		}
	if(false)
	for (auto& waypoint : _wayPoints)
	{
		glm::vec2 delta = glm::vec2(waypoint->getWorldPos().x, waypoint->getWorldPos().y) - prevWaypointPos;
		lenghtbetweenWaypoints += (glm::length(delta));
		//check if we already are on time to draw this 
		if (_startTimeWave<*_snapTime && lenghtbetweenWaypoints>lengthTraveled)
			waypoint->Draw();
	}
	if (_mainWaypointDirection != NULL)
		_mainWaypointDirection->Draw();
}
void EnemyWave::setAsMainWave()
{
	//cout << "DRAW WAYPOINT" << endl;
	for (auto &waypoint : _wayPoints)
	{
		
		waypoint->MainWaypoint();
	}
	_mainWave = true;
}
void EnemyWave::setAsSecondaryWave()
{
	//cout << "DRAW WAYPOINT" << endl;
	for (auto &waypoint : _wayPoints)
	{
		waypoint->SecondaryWaypoint();
	}
	_mainWave = false;
}
//Spawn one enemy at the first waypoint of the list
void EnemyWave::SpawnEnemy(World * pWorld, GameObject * pWaveParent)
{	//Check if we finished spawning all the enmies of this wave
	if (_quantitySpawnedEnemies < _sizeWave)
	{
		if (_wayPoints.size() == 0) { //If not waypoints return
			//cout << "No Waypoints... Abort..." << endl;
			return;
		}
		Waypoint * SpawnWaypoint = _wayPoints.at(0);
		glm::vec3 posplayerglmvec3 = pWorld->getMainPlayer()->getWorldPosition();
		Waypoint * playerPos= new Waypoint (glm::vec3(posplayerglmvec3.x, posplayerglmvec3.y, posplayerglmvec3.z), sf::Vector2f(500, 500), 1, 580, SpawnWaypoint->getRenderWindow());

		switch (_enemyBehaviour) {
		case 0: //Default, Basic AI
			break;
		case 1: //Kamikase AI

			_wayPoints.clear();
			_wayPoints.push_back(SpawnWaypoint);
			_wayPoints.push_back(playerPos);
			break;
		case 2: // Boss AI
			cout << "add Boss behaviour" << endl;
			
			break;
		}
		//Max points in Camera screen/Pixels*position of waypoint in screen-half the Max points in camera screen
		glm::vec3 pos = _wayPoints.at(0)->getWorldPos();
		glm::vec3 stepMovingMain = glm::vec3(0, 0, 0);
		if (_mainWaypointDirection != NULL) {
			float lengthMovingWaypoint = glm::length(_mainWaypointDirection->getWorldPos() - pos);
			stepMovingMain = glm::normalize(_mainWaypointDirection->getWorldPos() - pos)*(lengthMovingWaypoint / _sizeWave) * _quantitySpawnedEnemies;
			pos = pos +stepMovingMain;
		}
		pos.y = 0;
		//Enemy* Enemy1 = new Enemy("Enemy", pos);
		//AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "potato.png"));

		//Enemy* Enemy1 = new Enemy("Enemy", pos);
		ControlledActor* Enemy1 = new ControlledActor(pWorld, "Enemy", pos, new btSphereShape(5), ActorType::Type_Enemy, 1,  CF::COL_ENEMY, CF::enemyCollidesWith);
		
		//cout << "ENEMY CREATED" << endl;
		AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png"));

		Enemy1->setMesh(pWorld->GetResourceManager()->getMesh(Meshes::ID( _enemyType)));
		Enemy1->setMaterial(pWorld->GetResourceManager()->getMaterial(_enemyType));
		
		cout << "Material set" << endl;
		if (_enemyBehaviour != 2) {
			if (!_editorMode)
			{
				EnemyBehaviour* behave = new EnemyBehaviour(&_wayPoints, stepMovingMain);
				behave->setSpeed(_speed);
				behave->setShootRatio(_shootRatio);
				behave->setEnemyType(_enemyType);
				Enemy1->SetHealth(_health);
				Enemy1->setActorBehaviour(behave);
			}
			else if (_editorMode)
			{
				EnemyBehaviour* behave = new EnemyBehaviour(&_wayPoints, _snapTime, stepMovingMain);
				behave->setSpeed(_speed);
				behave->setShootRatio(_shootRatio);

				behave->setEnemyType(_enemyType);
				Enemy1->SetHealth(_health);
				Enemy1->setActorBehaviour(behave);

				behave->SaveOriginalTransform();

			}
		}
		else
		{
			BossBehaviour * Boss = new BossBehaviour(&_wayPoints, stepMovingMain,pWorld);
			Boss->setSpeed(_speed);
			Boss->setShootRatio(_shootRatio);

			Boss->setEnemyType(_enemyType);
			Enemy1->SetHealth(_health);
			Enemy1->setActorBehaviour(Boss);

		//	Boss->SaveOriginalTransform()
		}

		//Enemy1->scale(glm::vec3(2, 2, 2));
		//cout << "Scale set" << endl;
		pWaveParent->add(Enemy1);
		//cout << "Parent added" << endl;

		_quantitySpawnedEnemies++;
	}
}

bool EnemyWave::CheckSpawnTimeNextEnemy(float* pSec)
{
	_snapTime = pSec;
	if (*pSec - _timeAtLastEnemySpawned - _startTimeWave >= _delayBetweenEnemies)
	{
		_startTimeWave = 0;

		_timeAtLastEnemySpawned = *pSec;
		return true;
	}
	else 
		return false;
}
//Maybe Not used, _snapTime saved in CheckSpawnTimeNextEnemy()
void EnemyWave::UpdateEditorModePosition(float *pSec)
{
	_snapTime = pSec;
}

void EnemyWave::TestRealTime()
{
	_editorMode = false;
}

void EnemyWave::TestEditorMode()
{
	_editorMode = true;
}

#pragma region getters
const float * EnemyWave::getStartTime() const
{
	//cout << "START TIME !!!!!!!" << _startTimeWave << endl;
	return &_startTimeWave;
}

const int * EnemyWave::getSizeWave() const
{
	return &_sizeWave;
}

const float * EnemyWave::getDelayBetweenEnemies() const
{
	return &_delayBetweenEnemies;
}

const float * EnemyWave::getSpeed() const
{
	return &_speed;
}

const float * EnemyWave::getShootRatio() const
{
	return &_shootRatio;
}
const Materials::ID * EnemyWave::getEnemyType() const
{
	return &_enemyType;
}
const int * EnemyWave::getEnemyBehaviour() const
{
	return &_enemyBehaviour;
}
const float * EnemyWave::getHealth() const
{
	return &_health;
}
#pragma endregion

#pragma region Setters
void EnemyWave::setWaypoints(std::vector<Waypoint*> pWaypoints)
{
	_wayPoints = pWaypoints;
}

void EnemyWave::setStartTime(float pStartTime)
{
	_startTimeWave = pStartTime;
}

void EnemyWave::setSizeWave(int pSizeWave)
{
	_sizeWave = pSizeWave;
}

void EnemyWave::setDelayBetweenEnemies(float pDelayEnemies)
{
	_delayBetweenEnemies = pDelayEnemies;
}

void EnemyWave::setSpeed(float pSpeed)
{
	_speed = pSpeed;
}

void EnemyWave::setShootRatio(float pRatio)
{
	_shootRatio = pRatio;
}

void EnemyWave::setEnemyType(Materials::ID pType)
{
	_enemyType = pType;
}

void EnemyWave::setEnemyBehaviour(float pBehaviour)
{
	_enemyBehaviour = (int)pBehaviour;
}

void EnemyWave::setHealth(float pHealth)
{
	_health = pHealth;
}
#pragma endregion