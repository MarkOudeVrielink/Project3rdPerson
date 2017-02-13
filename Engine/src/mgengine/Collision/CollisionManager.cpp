#include "mgengine\Collision\CollisionManager.h"

#include <iostream>

CollisionManager::CollisionManager()
{
	_collisionConfig	= new btDefaultCollisionConfiguration();
	_dispatcher			= new btCollisionDispatcher(_collisionConfig);
	_broadphase			= new btDbvtBroadphase();
	_solver				= new btSequentialImpulseConstraintSolver();

	_physicsWorld		= new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfig);
	_physicsWorld->setGravity(btVector3(0,0,0));

	//gContactAddedCallback = CallBackDispatcher();
}

CollisionManager::~CollisionManager()
{
	delete _collisionConfig;
	delete _dispatcher;
	delete _broadphase;
	delete _solver;
	delete _physicsWorld;
}

void CollisionManager::AddCollisionActor(btRigidBody* pBody) {
	//TODO: Check if the given actor isn't already present in the vector.

	_physicsBodies.push_back(new physicsObject(pBody, 0));
	_physicsWorld->addRigidBody(pBody);	

	pBody->setUserPointer(_physicsBodies[_physicsBodies.size()-1]);
}

void CollisionManager::RemoveCollisionActor(btRigidBody* pBody) {
	//TODO: remove given Actor from the vector.
	_physicsWorld->removeRigidBody(pBody);
}

void CollisionManager::SimulatePhysics(float pTimeStep) {
	_physicsWorld->stepSimulation(pTimeStep);
}

bool CollisionManager::CallBackDispatcher(btManifoldPoint & pCollisionPoint, const btCollisionObjectWrapper * pObject1, int id1, int index1, const btCollisionObjectWrapper * pObject2 , int id2, int index2)
{
	std::cout << "collision" << std::endl;
	return false;
}
