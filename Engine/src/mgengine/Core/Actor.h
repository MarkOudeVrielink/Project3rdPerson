#ifndef ACTOR_H
#define ACTOR_H

#include "btBulletDynamicsCommon.h"
#include "mge\core\GameObject.hpp"
#include "mgengine\Behaviours\AbstractActorBehaviour.h"
//#include "mgengine\Collision\CollisionFilters.h"

//namespace CF {
//
//#define BIT(x) (1<<(x))
//
//	enum collisiontypes {
//		COL_NOTHING = 0,
//		COL_PLAYER	= BIT(0),
//		COL_ENEMY	= BIT(1),
//		COL_BULLET	= BIT(2),
//		COL_PICKUP	= BIT(3)
//	};
//
//	short playerCollidesWith	= COL_BULLET | COL_ENEMY | COL_PICKUP;
//	short enemyCollidesWith		= COL_BULLET | COL_PLAYER;
//	short bulletCollidesWith	= COL_PLAYER | COL_ENEMY;
//	short pickupCollidesWith	= COL_PLAYER;
//}

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
			float pMass					= 0.0f); 
	~Actor();

	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);	

	btRigidBody* GetRigidBody();

	AbstractActorBehaviour* getActorBehaviour() const;
	void setActorBehaviour(AbstractActorBehaviour* pBehaviour);

	World* GetWorld();
	ActorType GetType();
	
protected:
//	void initRigidBody(btCollisionShape* pCollider, CF::collisiontypes pType, short pMask);
	void initRigidBody(btCollisionShape* pCollider);
	void RemoveRigidBodyFromWorld();
	void AjustPosition();

	glm::mat4 floatToMat4(float* pMatrix);	

	AbstractActorBehaviour* _actorBehaviour;
	World*					_world;
	ActorType				_type;
	btRigidBody*			_rigidBody;

	float					_mass;
	
};

#endif // !ACTOR_H
