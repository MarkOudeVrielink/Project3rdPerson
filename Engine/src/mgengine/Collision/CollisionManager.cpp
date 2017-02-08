#include "mgengine\Collision\CollisionManager.h"


CollisionManager::CollisionManager()
{
	_collisionConfig	= new btDefaultCollisionConfiguration();
	_dispatcher			= new btCollisionDispatcher(_collisionConfig);
	_broadphase			= new btDbvtBroadphase();
	_solver				= new btSequentialImpulseConstraintSolver();

	_physicsWorld		= new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfig);
}

CollisionManager::~CollisionManager()
{
}