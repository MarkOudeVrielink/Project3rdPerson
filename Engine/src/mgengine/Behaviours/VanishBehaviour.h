#ifndef VANISHBEHAVIOUR_H
#define VANISHBEHAVIOUR_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include "mgengine\Core\Animation.h"

#include <string>

class VanishBehaviour : public AbstractActorBehaviour {
public:
	VanishBehaviour(std::string pFileName);
	~VanishBehaviour();

	void update(float pStep);
	void OnCollision();
	void setup();
	
private:

	sf::Texture	 _texture;
	Animation*	 _animation;		
};
#endif // !VANISHBEHAVIOUR_H

