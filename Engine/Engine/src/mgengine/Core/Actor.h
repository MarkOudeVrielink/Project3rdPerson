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

__declspec(align(16))class Actor : public GameObject {
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

	btQuaternion	getRotation();
	void			setRotation(glm::vec3 pAxis, btScalar pAngle);
	void			Slerp(glm::vec3 pAxis, btScalar pAngle);	
	void			Slerp(glm::vec3 pAxis, btScalar pAngle, btScalar pSlerpRate);
	//void			SetRotation(btQuaternion pRotation, glm::vec3 pAxis, btScalar pAngle);
	void	Destroy();

	virtual void Reset();
	virtual void ReCreate(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass);

	/*To make sure it alligns properly on the heap when dynamically allocating it. Otherwise the compiler cannot guarantee correct memory usage.*/
	void*	operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}
	void	operator delete(void* p)
	{
		_mm_free(p);
	}
protected:
	void _initRigidBody(btCollisionShape* pCollider);
	void _initRigidBody(btCollisionShape* pCollider, short pGroup, short pMask);	
	void _removeRigidBodyFromWorld();
	void _ajustPosition();
		
	virtual void _setDirty();

	glm::mat4 _floatToMat4(float* pMatrix);	

	AbstractActorBehaviour* _actorBehaviour;
	World*					_world;
	ActorType				_type;
	btRigidBody*			_rigidBody;	

	float					_mass;

	btQuaternion			_rotation;
};

#endif // !ACTOR_H
