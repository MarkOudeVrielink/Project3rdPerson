#include "mgengine\Collision\CollisionManager.h"

#include <iostream>
#include <iterator>

CollisionManager::CollisionManager()
{
	_collisionConfig	= new btDefaultCollisionConfiguration();
	_dispatcher			= new btCollisionDispatcher(_collisionConfig);
	_broadphase			= new btDbvtBroadphase();
	_solver				= new btSequentialImpulseConstraintSolver();

	_physicsWorld		= new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfig);
	_physicsWorld->setGravity(btVector3(0,0,0));	
}

CollisionManager::~CollisionManager()
{
	delete _collisionConfig;
	delete _dispatcher;
	delete _broadphase;
	delete _solver;
	delete _physicsWorld;
}

/*Add the rigidbody to the physicsworld*/
void CollisionManager::AddCollisionActor(btRigidBody * pBody, Actor * pActor)
{
	_physicsBodies.push_back(new physicsObject(pBody, pActor));
	_physicsWorld->addRigidBody(pBody);

	pBody->setUserPointer(_physicsBodies[_physicsBodies.size() - 1]);
}

/*Adds the rigidbody to the physicsworld.
	@param pGroup: the collision group the actor belongs to.
	@param pMask: the collision mask stores the collisions the group collides with.
*/
void CollisionManager::AddCollisionActor(btRigidBody* pBody, Actor* pActor, short pGroup, short pMask) {
	//TODO: Check if the given actor isn't already present in the vector.	
	_physicsBodies.push_back(new physicsObject(pBody, pActor));
	_physicsWorld->addRigidBody(pBody, pGroup, pMask);	
	
	pBody->setUserPointer(_physicsBodies[_physicsBodies.size()-1]);	
}

void CollisionManager::RemoveCollisionActor(btRigidBody* pBody) {
	//TODO: remove given Actor from the vector.
	_physicsWorld->removeRigidBody(pBody);
}

void CollisionManager::SimulatePhysics(float pTimeStep) {
	_physicsWorld->stepSimulation(pTimeStep);	
}

void CollisionManager::CheckCollisions()
{	
	/* Browse all collision pairs */
	int numManifolds = _physicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{			
		btPersistentManifold* contactManifold = _physicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		
		if (obA != nullptr && obB != nullptr) {
			/* Check all contacts points */
			int numContacts = contactManifold->getNumContacts();
			for (int j = 0; j<numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f)
				{				
					((physicsObject*)obA->getUserPointer())->actor->OnCollision(((physicsObject*)obB->getUserPointer())->actor);
					((physicsObject*)obB->getUserPointer())->actor->OnCollision(((physicsObject*)obA->getUserPointer())->actor);					
				}
			}
		}
	}
}

