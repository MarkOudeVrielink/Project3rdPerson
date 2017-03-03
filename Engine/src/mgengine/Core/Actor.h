#ifndef ACTOR_H
#define ACTOR_H

#include "btBulletDynamicsCommon.h"
#include "mge\core\GameObject.hpp"
#include "mgengine\Behaviours\AbstractActorBehaviour.h"

#include <iostream>
#include <SFML/System/Thread.hpp>

class World;

enum ActorType {
	Type_Player, Type_Enemy, Type_PickUp, Type_Bullet, Type_Nova, Type_StaticObject
};

class Actor : public GameObject {
public:
	Actor(	World* pWorld, 
			std::string pName, 
			glm::vec3 pPosition, 
			btCollisionShape* pCollider = new btSphereShape(1.0f), 
			ActorType pType				= ActorType::Type_StaticObject,
			float pMass					= 1.0f); 
	
	Actor(	World* pWorld,
			std::string pName,
			glm::vec3 pPosition,
			btCollisionShape* pCollider = new btSphereShape(1.0f),
			ActorType pType				= ActorType::Type_StaticObject,
			short pCollisionGroup		= 0,
			short pCollisionMask		= 0,
			float pMass					= 1.0f);
	~Actor();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);	

	btRigidBody* GetRigidBody();

	AbstractActorBehaviour* getActorBehaviour() const;
	void setActorBehaviour(AbstractActorBehaviour* pBehaviour);

	World*		getWorld();
	ActorType	getType();

	void SetRotation(glm::vec3 pAxis, btScalar pAngle);	
	void Destroy();

protected:
	void _initRigidBody(btCollisionShape* pCollider);
	void _initRigidBody(btCollisionShape* pCollider, short pGroup, short pMask);	
	void _removeRigidBodyFromWorld();
	void _ajustPosition();
		
	void _setDirty();

	glm::mat4 _floatToMat4(float* pMatrix);	

	AbstractActorBehaviour* _actorBehaviour;
	World*					_world;
	ActorType				_type;
	btRigidBody*			_rigidBody;	

	float					_mass;	
};

#endif // !ACTOR_H
