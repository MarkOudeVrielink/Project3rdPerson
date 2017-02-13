#ifndef ACTORMOVEMENT_H
#define ACTORMOVEMENT_H

#include "AbstractActorBehaviour.h"
class Mesh;
class AbstractMaterial;

class ActorMovement : public AbstractActorBehaviour {
public:
	ActorMovement(Mesh* pMesh, AbstractMaterial* pMatrial, float pSpeed = 4.0f);
	virtual ~ActorMovement();	
	
	virtual void update(float pStep);
		
private:
	void SpawnBullet();

	float _timer;
	float _fireRate;
	bool _fired;
	bool _overheat;

	float _coolDownTime;
	float _heat;

	float _moveSpeed;	
	Mesh* _mesh;
	AbstractMaterial* _material;
};

#endif // !ACTORMOVEMENT_H

