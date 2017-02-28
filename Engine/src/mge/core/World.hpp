#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Resources/ResourceManager.h"

#include <string>

class Camera;

class World : public GameObject
{
	public:
        World();	

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
		
		ResourceManager* GetResourceManager();
		CollisionManager* GetCollisionManager();

	private:
	    Camera* _mainCamera;	
		
		CollisionManager*	_physicsManager;
		ResourceManager*	_resourceManager;

        World(const World&);
        World& operator=(const World&);
};

#endif // WORLD_H
