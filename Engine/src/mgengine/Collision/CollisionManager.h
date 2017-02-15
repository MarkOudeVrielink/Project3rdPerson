#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <map>
#include "btBulletDynamicsCommon.h"
#include "mgengine\Core\Actor.h"

struct physicsObject {	
	btCollisionObject*	body;
	Actor*				actor;
	physicsObject(btCollisionObject* b, Actor* a) : actor(a), body(b) {}
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void AddCollisionActor(btRigidBody* pBody, Actor* pActor);
	void AddCollisionActor(btRigidBody* pBody, Actor* pActor, short pGroup, short pMask);
	void RemoveCollisionActor(btRigidBody* pBody);

	void SimulatePhysics(float pTimeStep);
	void CheckCollisions();

private:
	btDiscreteDynamicsWorld*	_physicsWorld;
	btBroadphaseInterface*		_broadphase;
	btCollisionConfiguration*	_collisionConfig;
	btDispatcher*				_dispatcher;
	btConstraintSolver*			_solver;

	std::vector<physicsObject*>	_physicsBodies;
};

#endif // !COLLISIONMANAGER_H

