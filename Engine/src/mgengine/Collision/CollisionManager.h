#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include "btBulletDynamicsCommon.h"

struct physicsObject {
	int					id;
	bool				hit;
	btCollisionObject*	body;
	physicsObject(btCollisionObject* b, int i) : body(b), id(i), hit(false) {}
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void AddCollisionActor(btRigidBody* pBody);
	void RemoveCollisionActor(btRigidBody* pBody);

	void SimulatePhysics(float pTimeStep);

	bool CallBackDispatcher(btManifoldPoint& pCollisionPoint,const btCollisionObjectWrapper* pObject1, int id1, int index1, const btCollisionObjectWrapper* pObject2, int id2, int index2);

private:
	btDiscreteDynamicsWorld*	_physicsWorld;
	btBroadphaseInterface*		_broadphase;
	btCollisionConfiguration*	_collisionConfig;
	btDispatcher*				_dispatcher;
	btConstraintSolver*			_solver;

	std::vector<physicsObject*>	_physicsBodies;
};

#endif // !COLLISIONMANAGER_H

