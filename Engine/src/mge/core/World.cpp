#include "mge/core/World.hpp"

using namespace std;

World::World():GameObject("root"), _mainCamera(0)
{
	//ctor
	physicsManager = new CollisionManager();
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}
