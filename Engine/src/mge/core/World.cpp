#include "mge/core/World.hpp"
#include "mgengine/Core/Actor.h"
#include <iterator>
#include <algorithm>

using namespace std;

World::World():GameObject("root"), _mainCamera(0), _dirtyActors()
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

ResourceManager* World::GetResourceManager()
{
	return _resourceManager;
}

CollisionManager* World::GetCollisionManager()
{
	return _physicsManager;
}

void World::setRenderWindow(sf::RenderWindow * pWindow)
{
	_window = pWindow;
}

sf::RenderWindow* World::getRenderWindow()
{
	return _window;
}

/*Destroy all the actors that were set to dirty.*/
void World::DestroyActors()
{
	while (!_dirtyActors.empty()) {
		delete _dirtyActors.back();
		_dirtyActors.pop_back();
	}
}

void World::SetDirtyActor(Actor* pActor)
{
	bool contains = (std::find(_dirtyActors.begin(), _dirtyActors.end(), pActor) != _dirtyActors.end());
	if (!contains) {
		_dirtyActors.push_back(pActor);
	}
}
