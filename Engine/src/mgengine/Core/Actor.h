#ifndef ACTOR_H
#define ACTOR_H

#include "btBulletDynamicsCommon.h"
#include "mge\core\GameObject.hpp"
#include "mgengine\Behaviours\AbstractActorBehaviour.h"

class World;

class Actor : public GameObject {
public:
	Actor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, float pMass);
	~Actor();

	virtual void update(float pStep);

	btRigidBody* GetRigidBody();

	AbstractActorBehaviour* getActorBehaviour() const;
	void setActorBehaviour(AbstractActorBehaviour* pBehaviour);

	World* GetWorld();
protected:
	void initRigidBody(btCollisionShape* pCollider);
	void AjustPosition();

	glm::mat4 floatToMat4(float* pMatrix);	

	AbstractActorBehaviour* _actorBehaviour;

	World* _world;

	btRigidBody* _rigidBody;
	float _mass;
	void RemoveRigidBodyFromWorld();

private:
};

#endif // !ACTOR_H
