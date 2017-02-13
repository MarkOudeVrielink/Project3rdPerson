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

void AbstractActorBehaviour::SetOwnerBody(btRigidBody * pBody)
{
	_ownerBody = pBody;
}
