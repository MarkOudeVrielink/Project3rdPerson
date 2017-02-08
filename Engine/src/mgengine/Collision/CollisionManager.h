#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>

#include "btBulletDynamicsCommon.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

private:
	btDiscreteDynamicsWorld*	_physicsWorld;
	btBroadphaseInterface*		_broadphase;
	btCollisionConfiguration*	_collisionConfig;
	btDispatcher*				_dispatcher;
	btConstraintSolver*			_solver;

	std::vector<btRigidBody*>	_rigidBodies;
};

#endif // !COLLISIONMANAGER_H

