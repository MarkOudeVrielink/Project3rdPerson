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
	GameObject *PlayerDefault = new GameObject("player_default");
	setMainPlayer(PlayerDefault);
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}
void World::setMainPlayer(GameObject* pPlayer) {
	if (pPlayer != NULL) _player = pPlayer;
}
GameObject* World::getMainPlayer() {
	return _player;
}
ResourceManager * World::GetResourceManager()
{
	return _resourceManager;
}

CollisionManager * World::GetCollisionManager()
{
	return _physicsManager;
}

/*Destroy all the actors that were set to dirty.*/
void World::DestroyActors()
{
	
	while (!_dirtyActors.empty()) {
		try {
			delete _dirtyActors.back();
			_dirtyActors.pop_back();
		}
		catch (int e)
		{

		}
	}
}

void World::SetDirtyActor(Actor* pActor)
{
	bool contains = (std::find(_dirtyActors.begin(), _dirtyActors.end(), pActor) != _dirtyActors.end());
	if (!contains) {
		_dirtyActors.push_back(pActor);
	}
}
