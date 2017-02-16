#ifndef ACTORMOVEMENT_H
#define ACTORMOVEMENT_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

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

	float	_timer;
	float	_fireRate;
	bool	_fired;
	bool	_overheat;

	float	_coolDownTime;
	float	_heat;
	float	_charge;
	float	_maxCharge;
	float	_beginNova;
	float	_endNova;
	float	_score;

	float				_moveSpeed;	
	Mesh*				_mesh;
	AbstractMaterial*	_material;
};

#endif // !ACTORMOVEMENT_H

