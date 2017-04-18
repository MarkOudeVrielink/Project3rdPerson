#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Resources/ResourceManager.h"

#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"

#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include "Light.h"

class Camera;
class Actor;
class HUD;

class World : public GameObject
{
	public:
        World();	
		std::list<Light*> *getLights();
		virtual void add(GameObject *pChild) override;
		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

		void setMainPlayer(GameObject * pPlayer);
		GameObject * getMainPlayer();
		
		ResourceManager*	GetResourceManager();
		CollisionManager*	GetCollisionManager();
		
		void				setRenderWindow(sf::RenderWindow* pWindow);
		sf::RenderWindow*	getRenderWindow();

		HUD*				getHud();

		void setPlayerDead(bool pState);

		bool getPlayerDead();

		void setDialogue(bool pBool, int pIndex);

		bool getDialogue(int pIndex);

		bool getDialogueEnded(int pIndex);

		void setDialogueEnded(bool pBool, int pIndex);
		////////////Enemy Bio
		void setEnemyBio(bool pBool, int pIndex);

		bool getEnemyBio(int pIndex);

		bool getEnemyBioEnded(int pIndex);

		void setEnemyBioEnded(bool pBool, int pIndex);

		void DestroyActors();
		void ResetObjects();
		void setBossDeath(bool pState);
		bool getBossDeath();
		void SetDirtyActor(Actor* pActor);

		void ResetObject(GameObject* pObject);
		void ResetObject(ControlledActor* pObject);
		void ResetObject(ObjectActor* pObject);

		
	protected:
	std::list<Light*> * _lights;

	private:
	    Camera* _mainCamera;	

		bool _playerDead = false;
		bool _bossDeath = false;

		HUD*	_hud;
		void	_initializeHud();
		

		CollisionManager*	_physicsManager;
		ResourceManager*	_resourceManager;
		GameObject* _player;
		
		std::list<Actor*>	_dirtyActors;	

		std::list<GameObject*>		_gameObjects;
		std::list<ControlledActor*> _controlledActors;
		std::list<ObjectActor*>		_objectActors;

		sf::RenderWindow*	_window;

        World(const World&);
        World& operator=(const World&);

		bool _dialoguePreGame = false;
		bool _dialoguePreBos = false;
		bool _dialoguePosWin = false;


		bool _dialoguePreGameEnded = false;
		bool _dialoguePreBosEnded = false;
		bool _dialoguePosWinEnded = false;


		bool _enemyBio1 = false;
		bool _enemyBio2 = false;
		bool _enemyBio3 = false;
		bool _enemyBio4 = false;
		bool _enemyBio5 = false;
		bool _enemyBio6 = false;

		bool _enemyBio1Ended = false;
		bool _enemyBio2Ended = false;
		bool _enemyBio3Ended = false;
		bool _enemyBio4Ended = false;
		bool _enemyBio5Ended = false;
		bool _enemyBio6Ended = false;
};

#endif // WORLD_H
