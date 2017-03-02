#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Resources/ResourceManager.h"

#include <string>
#include <list>

class Camera;
class Actor;

class World : public GameObject
{
	public:
        World();	

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
		
		ResourceManager* GetResourceManager();
		CollisionManager* GetCollisionManager();

		void DestroyActors();
		void SetDirtyActor(Actor* pActor);
	private:
	    Camera* _mainCamera;	
		
		CollisionManager*	_physicsManager;
		ResourceManager*	_resourceManager;

		std::list<Actor*>	_dirtyActors;

        World(const World&);
        World& operator=(const World&);
};

#endif // WORLD_H
