#include "EnemyWave.h"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/config.hpp"
#include "mygame\Behaviours\EnemyBehaviour.h"
EnemyWave::EnemyWave()
{
	for (int i = 0; i < 5;i++)
	{
		Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
		_enemies.push_back(Enemy1);
	}
	_enemy = Mesh::load(config::MGE_MODEL_PATH + "potato.obj");//LAG HERE
}
//TODO: Set the delay between enemies relative to the speed of the enemies to avoid them colliding between them

EnemyWave::~EnemyWave()
{
	cout << "Destroying Enemy Wave...." << endl;
	cout << "WTF should not being destoyed" << endl;
}
//Add waypoint to the list of waypoints in this wave
void EnemyWave::addWaypoint(Waypoint * pWaypoint, float pSec)
{
	//Set when the wave willl appear based on the first waypoint
	if (_wayPoints.empty())
	{
		_startTimeWave = pSec;
		cout << "Waypoints Empty.... Adding waypoint" << _startTimeWave << endl;

	}
	else
		cout << "NOT EMPTY HUEUHUE" << endl;
	_wayPoints.push_back(pWaypoint);
}
//Return reference of the list of all the waypoints
const std::vector<Waypoint*>* EnemyWave::getWaypoints() const
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

		Enemy* Enemy1 = new Enemy("Enemy", pos);
		AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "potato.png"));
		Enemy1->setMesh(_enemy);
		cout << "Mesh set" << endl;
		Enemy1->setMaterial(textureMaterial2);
		cout << "Material set" << endl;
		if (!_editorMode)
		{
			Enemy1->setBehaviour(new EnemyBehaviour(&_wayPoints));
			cout << "Behaviour set" << endl;
		}
		else if (_editorMode)
		{
			EnemyBehaviour* behave = new EnemyBehaviour(&_wayPoints, _snapTime);
			cout << "Behaviour set" << endl;
			Enemy1->setBehaviour(behave);
			behave->SaveOriginalTransform();

		}
		//Enemy1->scale(glm::vec3(0.5f, .5f, .5f));
		cout << "Scale set" << endl;
		pWorld->add(Enemy1);
		cout << "World added" << endl;
		_quantitySpawnedEnemies++;
	}
}

bool EnemyWave::CheckSpawnTimeNextEnemy(float* pSec)
{
	_snapTime = pSec;
	if (*pSec - _timeAtLastEnemySpawned - _startTimeWave >= _delayBetweenEnemies)
	{
		cout << _startTimeWave << "<<delayed by" << endl;
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

const float * EnemyWave::getStartTime() const
{
	cout << "START TIME !!!!!!!" << _startTimeWave << endl;
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
