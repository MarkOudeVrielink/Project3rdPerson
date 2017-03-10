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
void World::setPlayerDead(bool pState)
{
	_playerDead = pState;
}
bool World::getPlayerDead()
{
	return _playerDead;
}
//1: pregame
//2: preboss
//3: pos game
void World::setDialogue(bool pBool, int pIndex)
{
	switch (pIndex) {
	case 1:
		_dialoguePreGame = pBool;
		break;
	case 2:
		_dialoguePreBos = pBool;
		break;
	case 3:
		_dialoguePosWin = pBool;
		break;
	}
}
bool World::getDialogue(int pIndex)
{
	switch (pIndex) {
	case 1:
		return _dialoguePreGame;
		break;
	case 2:
		return _dialoguePreBos;
		break;
	case 3:
		return _dialoguePosWin;
		break;
	}
}
bool World::getDialogueEnded(int pIndex)
{
	switch (pIndex) {
	case 1:
		return _dialoguePreGameEnded;
		break;
	case 2:
		return _dialoguePreBosEnded;
		break;
	case 3:
		return _dialoguePosWinEnded;
		break;
	}
}
void World::setDialogueEnded(bool pBool, int pIndex)
{
	switch (pIndex) {
	case 1:
		_dialoguePreGameEnded = pBool;
		break;
	case 2:
		_dialoguePreBosEnded = pBool;
		break;
	case 3:
		_dialoguePosWinEnded = pBool;
		break;
	}
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
void World::setBossDeath(bool pState)
{
	_bossDeath = pState;
}
bool World::getBossDeath()
{
	return _bossDeath;
}
void World::SetDirtyActor(Actor* pActor)
{
	bool contains = (std::find(_dirtyActors.begin(), _dirtyActors.end(), pActor) != _dirtyActors.end());
	if (!contains) {
		_dirtyActors.push_back(pActor);
	}
}
