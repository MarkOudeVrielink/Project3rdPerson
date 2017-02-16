#ifndef ENEMYBEHAVIOUR_H
#define ENEMYBEHAVIOUR_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

class Mesh;
class AbstractMaterial;

class ActorEnemyBehaviour : public AbstractActorBehaviour {
public:
	ActorEnemyBehaviour(Mesh* pDropMesh, AbstractMaterial* pDropMaterial);
	virtual ~ActorEnemyBehaviour();
	
	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	void SpawnDrop(int pAmount = 1);
	void SpawnBullet();
private:
	Mesh*				_dropMesh;
	AbstractMaterial*	_dropMaterial;

};

#endif // !ENEMYBEHAVIOUR_H

