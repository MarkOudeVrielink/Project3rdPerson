#ifndef ABSTRACTACTORBEHAVIOUR_H
#define ABSTRACTACTORBEHAVIOUR_H

class btRigidBody;
class Actor;

class AbstractActorBehaviour {
public:

	AbstractActorBehaviour();
	virtual ~AbstractActorBehaviour() = 0;

	virtual void SetOwner(Actor* pActor);
	void SetOwnerBody(btRigidBody* pBody);

	virtual void update(float pStep) = 0;
	virtual void OnCollision(Actor* pActor);

protected:

	Actor* _owner;
	btRigidBody* _ownerBody = nullptr;

private:

	AbstractActorBehaviour(const AbstractActorBehaviour&);
	AbstractActorBehaviour& operator=(const AbstractActorBehaviour&);

};
#endif // !ABSTRACTACTORBEHAVIOUR_H

