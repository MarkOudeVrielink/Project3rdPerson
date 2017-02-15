#ifndef BULLETBEHAVIOUR_H
#define BULLETBEHAVIOUR_H
	
#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include "btBulletDynamicsCommon.h"

enum Direction{Up, Down};
enum BulletOwner{Player, Enemy};

class BulletBehaviour : public AbstractActorBehaviour {
public:
	BulletBehaviour(float pSpeed, float pPower, float pLiveTime = 50.0f , Direction pDirection = Direction::Up, BulletOwner pOwner = BulletOwner::Player);
	virtual ~BulletBehaviour();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	BulletOwner GetOwner();
private:
	float _speed;
	float _power;
	float _liveTime;
	float _timer;

	btVector3 _force;

	Direction _direction;
	BulletOwner _bulletOwner;
};

#endif // !BULLETBEHAVIOUR_H

