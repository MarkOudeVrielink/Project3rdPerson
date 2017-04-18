#include "ObjectPool.h"

ObjectPool::ObjectPool()
{
}

ObjectPool::~ObjectPool()
{
	while (_gameObjects.size())
	{
		GameObject* object = _gameObjects.front();
		_gameObjects.pop_front();
		delete object;
	}

	while (_objectActors.size()) 
	{
		ObjectActor* object = _objectActors.front();
		_objectActors.pop_front();
		object->Destroy();
	}

	while (_controlledActors.size())
	{
		ControlledActor* object = _controlledActors.front();
		_controlledActors.pop_front();
		object->Destroy();
	}
}

ObjectPool * ObjectPool::getInstance()
{
	if (_instance == 0)
	{
		_instance = new ObjectPool();

	}
	return _instance;
}

GameObject * ObjectPool::getGameObject(std::string pName, glm::vec3 pPostition)
{
	if (_objectActors.empty())
	{	
		return new GameObject(pName, pPostition);
	}
	else
	{		
		GameObject* resource = _gameObjects.front();
		resource->ReCreate(pName, pPostition);
		_gameObjects.pop_front();
		return resource;
	}
}

ObjectActor * ObjectPool::getObjectActor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass)
{
	if (_objectActors.empty())
	{		
		return new ObjectActor(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass);
	}
	else
	{		
		ObjectActor* resource = _objectActors.front();
		resource->ReCreate(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass);
		_objectActors.pop_front();
		return resource;
	}
}

ControlledActor* ObjectPool::getControlledActor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass, float pHealth, float pPower)
{
	if (_controlledActors.empty())
	{		
		return new ControlledActor(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass, pHealth, pPower);
	}
	else
	{		
		ControlledActor* resource = _controlledActors.front();
		resource->ReCreate(pWorld, pName, pPosition, pCollider, pType, pCollisionGroup, pCollisionMask, pMass, pHealth, pPower);
		_controlledActors.pop_front();
		return resource;
	}
}

void ObjectPool::returnObject(GameObject * object)
{
	object->Reset();
	_gameObjects.push_back(object);
}

void ObjectPool::returnObject(ObjectActor * object)
{
	object->Reset();
	_objectActors.push_back(object);
}

void ObjectPool::returnObject(ControlledActor * object)
{
	object->Reset();
	_controlledActors.push_back(object);
}

ObjectPool* ObjectPool::_instance = 0;
