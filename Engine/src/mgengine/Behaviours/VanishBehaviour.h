#ifndef VANISHBEHAVIOUR_H
#define VANISHBEHAVIOUR_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

class VanishBehaviour : public AbstractActorBehaviour {
public:
	VanishBehaviour(float pLifeTime = 1.0f);
	~VanishBehaviour();

	void update(float pStep);
	void OnCollision();
	
private:

	float _lifeTime;
	float _timeLived;
};
#endif // !VANISHBEHAVIOUR_H

