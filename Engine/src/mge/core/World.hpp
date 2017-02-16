#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine\Collision\CollisionManager.h"

class Camera;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
		
		//TODO: make this neat again, you know with a dumb getter.
		CollisionManager* physicsManager;
	private:
	    Camera* _mainCamera;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
