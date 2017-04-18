#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <string>
#include <iostream>
#include <list>

#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"

class ObjectPool {
public:
	ObjectPool();
	~ObjectPool();

	static ObjectPool* getInstance();

	GameObject* getGameObject(
		std::string pName,
		glm::vec3 pPostition);

	ObjectActor* getObjectActor(
		World* pWorld,
		std::string pName,
		glm::vec3 pPosition,
		btCollisionShape* pCollider = new btSphereShape(1.0f),
		ActorType pType = ActorType::Type_StaticObject,
		short pCollisionGroup = 0,
		short pCollisionMask = 0,
		float pMass = 1.0f);

	ControlledActor* getControlledActor(
		World* pWorld,
		std::string pName,
		glm::vec3 pPosition,
		btCollisionShape* pCollider = new btSphereShape(1.0f),
		ActorType pType = ActorType::Type_StaticObject,
		short pCollisionGroup = 0,
		short pCollisionMask = 0,
		float pMass = 1.0f,
		float pHealth = 1.0f,
		float pPower = 1.0f);

	void returnObject(GameObject* object);
	void returnObject(ObjectActor* object);
	void returnObject(ControlledActor* object);

private:
	std::list<GameObject*>		_gameObjects;
	std::list<ObjectActor*>		_objectActors;
	std::list<ControlledActor*> _controlledActors;

	static ObjectPool*			_instance;
};
#endif // !OBJECTPOOL_H
