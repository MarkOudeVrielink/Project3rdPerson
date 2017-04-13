#pragma once
#include "mygame/Waypoint.h"
#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include "mgengine\Resources\ResourceIdentifiers.h"
#include "SFML\System.hpp"

#include <glm.hpp>
using namespace std;

class Mesh;
class AbstractMaterial;
class EnemyMaterial;

class EnemyBehaviour : public AbstractActorBehaviour
{
public:

	EnemyBehaviour(std::vector<Waypoint*>* pWayPoints, glm::vec3 pMovingStep);
	EnemyBehaviour(std::vector<Waypoint*>* pWayPoints, float * ptime, glm::vec3 pMovingStep);
	EnemyBehaviour(GameObject * pPlayer, bool pLevelEditor = false, float * ptime = NULL);
	virtual ~EnemyBehaviour();

	void			SaveOriginalTransform();
	virtual void	update(float pStep);
	virtual void	OnCollision(Actor* pOther);
	virtual void	setup();
	void			UpdateEditorMode(float pStep);
	
	void			setShootRatio(float pShootPerSec);
	void			setSpeed(float pSpeed);
	void			setEnemyType(Materials::ID pType);
	float			getShootRatio();
	float			getSpeed();
	Materials::ID	getEnemyType();

	void SpawnDrop(int pAmount = 1);
	void SpawnBullet();
	int getBulletTypeBasedOnEnemy(int pIndex);
	float getScale(int pIndex);
	void SpawnExplosion();
	void AiKamikase(float pStep);
		
private:
	std::vector<Waypoint*>* _wayPoints;
	Waypoint *_tarjet = nullptr;

	float			_moveSpeed;
	float			_turnSpeed;	
	float			_shootRatio =2;
	Materials::ID	_enemyType;
	int				_index = 0;
	bool			_levelEditorMode = true;
	
	//In SECONDS last snap sec moment
	float*	_referenceToArtificialLevelTime;
	float	_lastTimeElapsedEditorMode = 0;
	float	_diferenceBetweenLastMovement = 0;
	
	int lastSecShowed = -100;
	
	glm::mat4 _originalTransform;
	glm::mat4 _lastFrameTransform;
	glm::vec3 _up;

	void AiBasic(float pStep);
	void AiBasicBackWards(float pStep);

	float	_secondsAlreadyRendered = 0;
	float	_spawnedTime = 0;
	bool	_movingBackwards = false;
	glm::vec3 _movingStep;

	sf::Clock updateClock;
	sf::Time timeSinceLastShoot = sf::Time::Zero;

	btScalar			_angle;
	Mesh*				_dropMesh;
	AbstractMaterial*	_dropMaterial;
	EnemyMaterial*		_enemyMaterial;

	float		_animationTime = 0;
	sf::Clock	_animtionClock;
	void		_animate();

	//Mesh*				_dropMesh;
	//AbstractMaterial*	_dropMaterial;

	bool _kamikase = false;
	bool _kamikaseTargetSet = false;
	glm::vec3 _kamikaseTargetPos;
	GameObject * _player;
};

