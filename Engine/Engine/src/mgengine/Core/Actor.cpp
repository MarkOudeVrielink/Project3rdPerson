#include "Actor.h"
#include "mge\core\World.hpp"

#include <functional>
/*Creates an actor that can collide with anything.*/
Actor::Actor(World* pWorld,std::string pName, glm::vec3 pPosition, btCollisionShape* pCollider, ActorType pType, float pMass)
	: GameObject(pName, pPosition), _mass(pMass), _world (pWorld), _type(pType), _actorBehaviour(NULL)
{
	_initRigidBody(pCollider);	
}

/*Creates an actor that collides with what the specified group/mask is supposed to collide with.
E.g. group: player matches with mask: player. This way the player collides with the type of objects it's supposed to collide with.
*/
Actor::Actor(World * pWorld, std::string pName, glm::vec3 pPosition, btCollisionShape * pCollider, ActorType pType, short pCollisionGroup, short pCollisionMask, float pMass)
	: GameObject(pName, pPosition), _mass(pMass), _world(pWorld), _type(pType), _actorBehaviour(NULL)
{
	_initRigidBody(pCollider, pCollisionGroup, pCollisionMask);	
}

Actor::~Actor() {
	_removeRigidBodyFromWorld();
	
	if (_actorBehaviour){
		delete _actorBehaviour;
		_actorBehaviour = NULL;
	}		
}

void Actor::update(float pStep) {
	_ajustPosition();
	
	if (_actorBehaviour) 
		_actorBehaviour->update(pStep); 
}

btRigidBody* Actor::GetRigidBody() {
	if (_rigidBody)
		return _rigidBody;
	return	nullptr;
}


void Actor::_removeRigidBodyFromWorld()
{
	_world->GetCollisionManager()->RemoveCollisionActor(_rigidBody);
}

void Actor::setActorBehaviour(AbstractActorBehaviour* pBehaviour)
{
	_actorBehaviour = pBehaviour;
	_actorBehaviour->SetOwner(this);
	_actorBehaviour->SetOwnerBody(GetRigidBody());
	_actorBehaviour->setup();
}

World* Actor::getWorld()
{
	return _world;
}

ActorType Actor::getType()
{
	return _type;
}

btQuaternion Actor::getRotation()
{
	return _rotation;
}

void Actor::SetRotation(glm::vec3 pAxis, btScalar pAngle)
{
	//Create a quaternion with that's rotated towards the right angle.
	btQuaternion newRotation = _rigidBody->getWorldTransform().getRotation();
	btQuaternion oldrotation = newRotation;

	//Set the newRotation to the correct angle.
	newRotation.setRotation(btVector3(pAxis.x, pAxis.y, pAxis.z), pAngle);

	//Get the slerp quaternion, in otherwords get the rotation we have to set to.
	btQuaternion slerp = oldrotation.slerp(newRotation, 0.1f);
	
	//Get the objects current transform.
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(getWorldTransform()));

	//Set the new rotation.
	trans.setRotation(slerp);
	_rigidBody->setWorldTransform(trans);
}

/*Destroys this actor after the given amount of time.*/
void Actor::Destroy()
{		
	_setDirty();	
}

void Actor::OnCollision(Actor * pOther)
{
}

AbstractActorBehaviour* Actor::getActorBehaviour() const
{
	return _actorBehaviour;
}

void Actor::_ajustPosition() {	
	float mat[16];
	btTransform t;

	//Get rigidbody position.
	_rigidBody->getMotionState()->getWorldTransform(t);
	
	//Cast is to a glm::mat4 and set the OpenGL transform to that of the btTransform.
	t.getOpenGLMatrix(mat);		
	setTransform(glm::scale(_floatToMat4(mat), _scale));
}

void Actor::_setDirty()
{
	_world->SetDirtyActor(this);
}

void Actor::_initRigidBody(btCollisionShape * pCollider)
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
	_rotation = btT.getRotation();
}

void Actor::_initRigidBody(btCollisionShape* pCollider, short pGroup, short pMask) {
	//Cast our glm::mat4 to a btTransform. So we sync our rigidbody's position with that of the object.	
	btTransform btT;
	btT.setFromOpenGLMatrix(glm::value_ptr(_transform));
	
	//Setup the rigidbody of the actor.
	btMotionState* motion = new btDefaultMotionState(btT);
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, pCollider, btVector3(0,0,0));
	_rigidBody = new btRigidBody(info);
	_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	_world->GetCollisionManager()->AddCollisionActor(_rigidBody, this, pGroup, pMask);
	_rotation = btT.getRotation();
}

/*@param takes a 16-element float array an converts it to a glm::Mat4.*/
glm::mat4 Actor::_floatToMat4(float * pMatrix)
{
	return glm::mat4(	pMatrix[0],		pMatrix[1],		pMatrix[2],		pMatrix[3],
						pMatrix[4],		pMatrix[5],		pMatrix[6],		pMatrix[7],
						pMatrix[8],		pMatrix[9],		pMatrix[10],	pMatrix[11],
						pMatrix[12],	pMatrix[13],	pMatrix[14],	pMatrix[15]);
}