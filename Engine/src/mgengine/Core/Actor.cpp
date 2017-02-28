#include "Actor.h"
#include "mge\core\World.hpp"

/*Creates an actor that can collide with anything.*/
Actor::Actor(World* pWorld,std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, ActorType pType, float pMass)
	: GameObject(pName, pPosition), _mass(pMass), _world (pWorld), _type(pType), _actorBehaviour(NULL)
{
	initRigidBody(pCollider);
}

/*Creates an actor that collides with what the specified group/mask is supposed to collide with.
E.g. group: player matches with mask: player. This way the player collides with the type of objects it's supposed to collide with.
*/
Actor::Actor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass)
	: GameObject(pName, pPosition), _mass(pMass), _world(pWorld), _type(pType), _actorBehaviour(NULL)
{
	initRigidBody(pCollider, pCollisionGroup, pCollisionMask);
}

Actor::~Actor() {
	RemoveRigidBodyFromWorld();
	
	if (_actorBehaviour){
		delete _actorBehaviour;
		_actorBehaviour = NULL;
	}		
}

void Actor::update(float pStep) {
	AjustPosition();
	
	if (_actorBehaviour) 
		_actorBehaviour->update(pStep); 
}

btRigidBody* Actor::GetRigidBody() {
	if (_rigidBody)
		return _rigidBody;
	return	nullptr;
}


void Actor::RemoveRigidBodyFromWorld()
{
	_world->GetCollisionManager()->RemoveCollisionActor(_rigidBody);
}

void Actor::setActorBehaviour(AbstractActorBehaviour* pBehaviour)
{
	_actorBehaviour = pBehaviour;
	_actorBehaviour->SetOwner(this);
	_actorBehaviour->SetOwnerBody(GetRigidBody());
}

World* Actor::GetWorld()
{
	return _world;
}

ActorType Actor::GetType()
{
	return _type;
}

void Actor::SetRotation(glm::vec3 pAxis, btScalar pAngle)
{
	//Create a quaternion with that's rotated towards the right angle.
	btQuaternion newRotation;
	newRotation.setRotation(btVector3(pAxis.x, pAxis.y, pAxis.z), pAngle);

	//Get the objects current transform.
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(getWorldTransform()));

	//Set the new rotation.
	trans.setRotation(newRotation);
	_rigidBody->setWorldTransform(trans);
}

void Actor::OnCollision(Actor * pOther)
{
}

AbstractActorBehaviour* Actor::getActorBehaviour() const
{
	return _actorBehaviour;
}

void Actor::AjustPosition() {	
	float mat[16];
	btTransform t;

	//Get rigidbody position.
	_rigidBody->getMotionState()->getWorldTransform(t);
	
	//Cast is to a glm::mat4 and set the OpenGL transform to that of the btTransform.
	t.getOpenGLMatrix(mat);		
	setTransform(glm::scale(floatToMat4(mat), _scale));
}

void Actor::initRigidBody(btCollisionShape * pCollider)
{
	//Cast our glm::mat4 to a btTransform. So we sync our rigidbody's position with that of the object.
	btTransform btT;
	btT.setFromOpenGLMatrix(glm::value_ptr(_transform));

	//Setup the rigidbody of the actor.
	btMotionState* motion = new btDefaultMotionState(btT);
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, pCollider, btVector3(0, 0, 0));
	_rigidBody = new btRigidBody(info);
	_rigidBody->setActivationState(DISABLE_DEACTIVATION); //We disable this so it won't go to sleep as soon as the rigidbody stops moving.

	_world->GetCollisionManager()->AddCollisionActor(_rigidBody, this);
}

//TODO: Make it more ajustable.
void Actor::initRigidBody(btCollisionShape* pCollider, short pGroup, short pMask) {
	//Cast our glm::mat4 to a btTransform. So we sync our rigidbody's position with that of the object.	
	btTransform btT;
	btT.setFromOpenGLMatrix(glm::value_ptr(_transform));
	
	//Setup the rigidbody of the actor.
	btMotionState* motion = new btDefaultMotionState(btT);
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, pCollider, btVector3(0,0,0));
	_rigidBody = new btRigidBody(info);
	_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	_world->GetCollisionManager()->AddCollisionActor(_rigidBody, this, pGroup, pMask);

	//_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);	
}

/*@param takes a 16-element float array an converts it to a glm::Mat4.*/
glm::mat4 Actor::floatToMat4(float * pMatrix)
{
	return glm::mat4(	pMatrix[0],		pMatrix[1],		pMatrix[2],		pMatrix[3],
						pMatrix[4],		pMatrix[5],		pMatrix[6],		pMatrix[7],
						pMatrix[8],		pMatrix[9],		pMatrix[10],	pMatrix[11],
						pMatrix[12],	pMatrix[13],	pMatrix[14],	pMatrix[15]);
}