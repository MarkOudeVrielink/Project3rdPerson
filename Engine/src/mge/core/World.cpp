#include "mge/core/World.hpp"
#include "mgengine/Core/Actor.h"
#include "mgengine/UI/HUD.h"

#include "mge/config.hpp"

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
	_lights = new std::list<Light*>();
}



#pragma region get/set
std::list<Light*>* World::getLights()
{
	return _lights;
}
void World::add(GameObject* pChild)
{
	//check if is light and is in world 
	if (this->getName() == "root")
	{
		cout << "ROOT DETECTED" << endl;
	}
	if (this->getName() == "root" && typeid(*pChild) == typeid (Light))
	{
		cout << "LIGHT NOT IN ROOT" << endl;
		_lights->push_back((Light*)pChild);
	}
	else if (typeid(pChild) == typeid (Light))
	{
		cout << "LIGHT NOT IN ROOT" << endl;
		_lights->push_back((Light*)pChild);
	}
	GameObject::add(pChild);
}
void World::setMainCamera(Camera* pCamera) {
	if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera() {
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

CollisionManager* World::GetCollisionManager()
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

void World::setRenderWindow(sf::RenderWindow * pWindow)
{
	_window = pWindow;
	_initializeHud();
}

sf::RenderWindow* World::getRenderWindow()
{
	return _window;
}

void World::_initializeHud()
{
	_hud = new HUD(_window);
}

HUD * World::getHud()
{
	return _hud;
}
#pragma endregion

#pragma region Actor Gargbage collection

/*Destroy all the actors that were set to dirty.*/
void World::DestroyActors()
{
	while (!_dirtyActors.empty()) { 
		delete _dirtyActors.back();
		_dirtyActors.pop_back();
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



#pragma endregion

