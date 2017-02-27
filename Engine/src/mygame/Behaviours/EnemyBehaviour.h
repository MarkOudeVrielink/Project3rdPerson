#pragma once
#include "mygame/Waypoint.h"
#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include <glm.hpp>
using namespace std;

class Mesh;
class AbstractMaterial;

class EnemyBehaviour : public AbstractActorBehaviour
{
public:

	EnemyBehaviour(std::vector<Waypoint*> *pWayPoints);
	EnemyBehaviour(std::vector<Waypoint*> *pWayPoints, float * ptime);
	virtual ~EnemyBehaviour();

	void SaveOriginalTransform();
	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);
	void UpdateEditorMode(float pStep);
	
	void SpawnDrop(int pAmount = 1);
	//void SpawnBullet();
private:
	float _moveSpeed;
	float _turnSpeed;
	std::vector<Waypoint*>* _wayPoints;
	Waypoint *_tarjet = nullptr;
	int _index = 0;
	bool _levelEditorMode = true;
	//In SECONDS last snap sec moment
	float* _referenceToArtificialLevelTime;
	float _lastTimeElapsedEditorMode = 0;
	float _diferenceBetweenLastMovement = 0;
	int lastSecShowed = -100;
	glm::mat4 _originalTransform;
	glm::mat4 _lastFrameTransform;
	glm::vec3 _up;
	void AiBasic(float pStep);
	void AiBasicBackWards(float pStep);
	float _secondsAlreadyRendered = 0;
	float _spawnedTime = 0;
	bool _movingBackwards = false;

	btScalar _angle;
	//Mesh*				_dropMesh;
	//AbstractMaterial*	_dropMaterial;
};

