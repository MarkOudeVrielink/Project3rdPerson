#include <cassert>
#include <iostream>
#include "AbstractActorBehaviour.h"

AbstractActorBehaviour::AbstractActorBehaviour() : _owner(NULL) {}

AbstractActorBehaviour::~AbstractActorBehaviour() {
	//delete _owner;
}

void AbstractActorBehaviour::SetOwner(Actor* pOwner) {
	_owner = pOwner;
}

//This is used to setup things that can't be done in the contructor due to the way behaviours and the owner are setup.
void AbstractActorBehaviour::setup()
{
}

void AbstractActorBehaviour::SetOwnerBody(btRigidBody * pBody)
{
	_ownerBody = pBody;
}

void AbstractActorBehaviour::OnCollision(Actor * pActor)
{
	
}
