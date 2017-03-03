#ifndef BULLETBEHAVIOUR_H
#define BULLETBEHAVIOUR_H
	
#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include "btBulletDynamicsCommon.h"

enum Direction{Up, Down};
enum BulletOwner{Player, Enemy};

__declspec(align(16))class BulletBehaviour : public AbstractActorBehaviour {
public:
	BulletBehaviour(float pSpeed, float pPower = 1.0f, float pLiveTime = 1.0f , Direction pDirection = Direction::Up, BulletOwner pOwner = BulletOwner::Player);
	virtual ~BulletBehaviour();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	BulletOwner getOwner();
	float		getPower();
	
	/*To make sure it alligns properly on the heap when dynamically allocating it. Otherwise the compiler cannot guarantee correct memory usage.*/
	void*	operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}
	void	operator delete(void* p)
	{
		_mm_free(p);
	}
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

