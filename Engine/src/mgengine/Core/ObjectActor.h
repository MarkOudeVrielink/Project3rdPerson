#ifndef OBJECTACTOR_H
#define OBJECTACTOR_H

#include "mgengine\Core\Actor.h"

enum ObjectType {
	StaticObeject, Bullet
};

class ObjectActor : public Actor {
public :
	ObjectActor(World* pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, float pMass = 0, ObjectType pType = ObjectType::StaticObeject);
	~ObjectActor();

	virtual void update(float pStep);

	ObjectType GetType();
private:
	ObjectType _type;
};
#endif // !OBJECTACTOR_H

