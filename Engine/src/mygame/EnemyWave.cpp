#include "EnemyWave.h"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/config.hpp"
#include "mygame\Behaviours\EnemyBehaviour.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Collision\CollisionFilters.h"

EnemyWave::EnemyWave()
{
	for (int i = 0; i < 5;i++)
	{
		Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
		_enemies.push_back(Enemy1);
	}
	_enemy = Mesh::load(config::MGE_MODEL_PATH + "ship.obj");//LAG HERE
}
//TODO: Set the delay between enemies relative to the speed of the enemies to avoid them colliding between them

EnemyWave::~EnemyWave()
{
}
//Add waypoint to the list of waypoints in this wave
void EnemyWave::addWaypoint(Waypoint * pWaypoint, float pSec)
{
	if (_wayPoints.size() == 0)
		_startTimeWave = pSec;
	_wayPoints.push_back(pWaypoint);
}
//Return reference of the list of all the waypoints
std::vector<Waypoint*>* EnemyWave::getWaypoints()
{
	return &_wayPoints;
}
//Loop and draw all the waypoints in screen
void EnemyWave::DrawWaypoints()
{
	//cout << "DRAW WAYPOINT" << endl;
	for (auto &waypoint : _wayPoints)
	{
		waypoint->Draw();
	}
}
//Spawn one enemy at the first waypoint of the list
void EnemyWave::SpawnEnemy(World * pWorld)//TODO:change to Level scope
{	//Check if we finished spawning all the enmies of this wave
	if (_quantitySpawnedEnemies < _sizeWave)
	{
		if (_wayPoints.size() == 0) { //If not waypoints return
			cout << "No Waypoints... Abort..." << endl;
			return;
		}
		//Max points in Camera screen/Pixels*position of waypoint in screen-half the Max points in camera screen
		glm::vec3 pos = glm::vec3(160.0f / 1920 * _wayPoints.at(0)->getPosition().x - 80,
			0,
			80.0f / 1080 * _wayPoints.at(0)->getPosition().y - 40);

		//Enemy* Enemy1 = new Enemy("Enemy", pos);
		ControlledActor* Enemy1 = new ControlledActor(pWorld, "Enemy", pos, new btSphereShape(1), ActorType::Type_Enemy, 1,  CF::COL_ENEMY, CF::enemyCollidesWith);
		
		cout << "ENEMY CREATED" << endl;
		AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png"));
		Enemy1->setMesh(_enemy);
		Enemy1->setMaterial(textureMaterial2);

		if(!_editorMode)
			Enemy1->setActorBehaviour(new EnemyBehaviour(&_wayPoints));
		else if(_editorMode)
		{
			EnemyBehaviour* behave = new EnemyBehaviour(&_wayPoints, _snapTime);
			
			Enemy1->setActorBehaviour(behave);
			behave->SaveOriginalTransform();
			
		}
		Enemy1->scale(glm::vec3(0.5f, .5f, .5f));

		pWorld->add(Enemy1);
		_quantitySpawnedEnemies++;
	}
}

bool EnemyWave::CheckSpawnTimeNextEnemy(float* pSec)
{
	_snapTime = pSec;
	if (*pSec - _timeAtLastEnemySpawned-_startTimeWave >= _delayBetweenEnemies)
	{
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
