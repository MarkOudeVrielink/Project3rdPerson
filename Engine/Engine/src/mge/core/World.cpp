#include "mge/core/World.hpp"
#include "mgengine/Core/Actor.h"
#include "mgengine/UI/HUD.h"

#include "mgengine\Core\ObjectPool.h"

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
		cout << "" << endl;
	}
	if (this->getName() == "root" && typeid(*pChild) == typeid (Light))
	{
		cout << "" << endl;
		_lights->push_back((Light*)pChild);
	}
	else if (typeid(pChild) == typeid (Light))
	{
		cout << "" << endl;
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
///////////////////ENEMY BIOS////////////////////////////////
void World::setEnemyBio(bool pBool, int pIndex)
{
	switch (pIndex) {
	case 1:
		_enemyBio1 = pBool;
		break;
	case 2:
		_enemyBio2 = pBool;
		break;
	case 3:
		_enemyBio3 = pBool;
		break;
	case 4:
		_enemyBio4 = pBool;
		break;
	case 5:
		_enemyBio5 = pBool;
		break;
	case 6:
		_enemyBio6 = pBool;
		break;
	}
}
bool World::getEnemyBio(int pIndex)
{
	switch (pIndex) {
	case 1:
		return _enemyBio1;
		break;
	case 2:
		return _enemyBio2;
		break;
	case 3:
		return _enemyBio3;
		break;
	case 4:
		return _enemyBio4;
		break;
	case 5:
		return _enemyBio5;
		break;
	case 6:
		return _enemyBio6;
		break;
	}
}

bool World::getEnemyBioEnded(int pIndex)
{
	switch (pIndex) {
	case 1:
		return _enemyBio1Ended;
		break;
	case 2:
		return _enemyBio2Ended;
		break;
	case 3:
		return _enemyBio3Ended;
		break;
	case 4:
		return _enemyBio4Ended;
		break;
	case 5:
		return _enemyBio5Ended;
		break;
	case 6:
		return _enemyBio6Ended;
		break;
	}
}

void World::setEnemyBioEnded(bool pBool, int pIndex)
{
	switch (pIndex) {
	case 1:
		_enemyBio1Ended = pBool;
		break;
	case 2:
		_enemyBio2Ended = pBool;
		break;
	case 3:
		_enemyBio3Ended = pBool;
		break;
	case 4:
		_enemyBio4Ended  = pBool;
		break;
	case 5:
		_enemyBio5Ended = pBool;
		break;
	case 6:
		_enemyBio6Ended = pBool;
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
void World::ResetObjects()
{
	while (!_gameObjects.empty()) {
		ObjectPool::getInstance()->returnObject(_gameObjects.back());
		_gameObjects.pop_back();
	}

	while (!_controlledActors.empty()) {
		ObjectPool::getInstance()->returnObject(_controlledActors.back());
		_controlledActors.pop_back();
	}

	while (!_objectActors.empty()) {
		ObjectPool::getInstance()->returnObject(_objectActors.back());
		_objectActors.pop_back();
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

void World::ResetObject(GameObject * pObject)
{
	bool contains = (std::find(_gameObjects.begin(), _gameObjects.end(), pObject) != _gameObjects.end());
	if (!contains) {
		_gameObjects.push_back(pObject);
	}
}

void World::ResetObject(ControlledActor * pObject)
{
	bool contains = (std::find(_controlledActors.begin(), _controlledActors.end(), pObject) != _controlledActors.end());
	if (!contains) {
		_controlledActors.push_back(pObject);
	}
}

void World::ResetObject(ObjectActor * pObject)
{
	bool contains = (std::find(_objectActors.begin(), _objectActors.end(), pObject) != _objectActors.end());
	if (!contains) {
		_objectActors.push_back(pObject);
	}
}



#pragma endregion

