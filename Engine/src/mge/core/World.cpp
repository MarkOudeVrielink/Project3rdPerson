#include "mge/core/World.hpp"

using namespace std;

World::World():GameObject("root"), _mainCamera(0)
{
	//ctor
	_resourceManager = new ResourceManager();
	_physicsManager = new CollisionManager();
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

ResourceManager * World::GetResourceManager()
{
	return _resourceManager;
}

CollisionManager * World::GetCollisionManager()
{
	return _physicsManager;
}
