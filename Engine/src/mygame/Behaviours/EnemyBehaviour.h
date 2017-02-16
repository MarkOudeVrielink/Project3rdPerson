#pragma once
#include "mygame/Waypoint.h"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm.hpp>
using namespace std;
class EnemyBehaviour : public AbstractBehaviour
{
public:

	EnemyBehaviour(std::vector<Waypoint*> *pWayPoints);
	EnemyBehaviour(std::vector<Waypoint*> *pWayPoints, float * ptime);
	~EnemyBehaviour();
	void SaveOriginalTransform();
	virtual void update(float pStep);
	void UpdateEditorMode(float pStep);
	
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
};

