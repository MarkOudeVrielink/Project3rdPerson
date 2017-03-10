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

class BossBehaviour : public AbstractActorBehaviour
{
public:
	//BossBehaviour(std::vector<Waypoint*>* pWayPoints, glm::vec3 pMovingStep);
	BossBehaviour(std::vector<Waypoint*>* pWayPoints, glm::vec3 pMovingStep, World * pWorld);
	virtual void update(float pStep);
	void RotateAndshoot(float pStep);
	void SpawnEnemiesKamikase(float pStep);
	bool ChargePlayer(float pStep);
	bool GoToSpawnPosition(float pStep);
	void BounceInScreen(float pStep);
	virtual void	OnCollision(Actor* pOther);
	virtual void	setup();
	void SpawnBullet();
	void SpawnBullet(float pBulletPower, glm::vec3 pDirection, float pAngle);
	void SpawnExplosion();

	void			setShootRatio(float pShootPerSec);
	void			setSpeed(float pSpeed);
	void			setEnemyType(Materials::ID pType);
	float			getShootRatio();
	float			getSpeed();
	Materials::ID	getEnemyType();
private:
	enum BossState{Idle,RotateShoot,SpawnEnemies,Charge,Angry};
	BossState _bossState = Idle;
	std::vector<Waypoint*>* _wayPoints;
	std::vector<Waypoint*> _wayPointsKamikase;
	Waypoint *_tarjet = nullptr;

	glm::vec3 _movingStep;
	float			_moveSpeed;
	float			_turnSpeed;
	float			_shootRatio = 2;
	Materials::ID	_enemyType;
	int				_index = 0;
	bool			_levelEditorMode = true;

	bool AiBasic(float pStep);
	void AiBasicBackWards(float pStep);

	float				_angle2 =0;
	Mesh*				_dropMesh;
	AbstractMaterial*	_dropMaterial;
	EnemyMaterial*		_enemyMaterial;
	World* _world;

	sf::Clock shootClock;
	sf::Time timeSinceLastShoot = sf::Time::Zero;

	BossState _nextBehaviour = BossState::RotateShoot;

	sf::Clock behaviourClock;
	sf::Time timeSinceLastBehaviourChange = sf::Time::Zero;

	bool _playerDirectionSaved = false;
	bool chargedAgainstPlayer = false;
	float _angle;
	bool reachedPlayer = false;
	bool AiBasicDone = false;
	glm::vec2 delta;
	glm::vec3 _directionToPlayer;
	glm::vec3 _originalSpawn;
};