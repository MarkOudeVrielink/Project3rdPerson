#ifndef ACTORMOVEMENT_H
#define ACTORMOVEMENT_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

#include <glm.hpp>

class Mesh;
class AbstractMaterial;

class PlayerBehaviour : public AbstractActorBehaviour {
public:
	PlayerBehaviour(Mesh* pMesh, AbstractMaterial* pMatrial, float pSpeed = 4.0f);
	virtual ~PlayerBehaviour();	
	
	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);
	
private:
	void SpawnBullet(float pBulletPower);
	void SpawnNova();
	
	void FireWeapon(float pTime);
	void Move();
	void IsInvulnerable(float pTime);

	glm::vec3 _spawnOffset;

	bool	_invulnerable;
	float	_invulnerabilityTime;
	float	_invulnerabilityTimer;
	int		_defaultFlags;

	float	_weaponTimer;
	float	_fireRate;
	bool	_fired;
	bool	_overheat;

	float	_coolDownTime;
	float	_heat;
	float	_timeToOverheat;
	float	_coolDownRate;

	float	_charge;
	float	_maxCharge;
	
	float	_score;

	float				_tiltAngle;
	float				_moveSpeed;	
	Mesh*				_mesh;
	AbstractMaterial*	_material;
};

#endif // !ACTORMOVEMENT_H

