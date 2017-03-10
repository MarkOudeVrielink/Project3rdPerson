#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Resources/ResourceManager.h"

#include <string>
#include <list>
#include <SFML/Graphics.hpp>

class Camera;
class Actor;
class HUD;

class World : public GameObject
{
	public:
        World();	

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

		void DestroyActors();
		void setBossDeath(bool pState);
		bool getBossDeath();
		void SetDirtyActor(Actor* pActor);

		
	private:
	    Camera* _mainCamera;	
<<<<<<< HEAD
		bool _playerDead = false;
		bool _bossDeath = false;
=======
		HUD*	_hud;
		void	_initializeHud();
		
>>>>>>> origin/master
		CollisionManager*	_physicsManager;
		ResourceManager*	_resourceManager;
		GameObject* _player;

		std::list<Actor*>	_dirtyActors;		

		sf::RenderWindow*	_window;

        World(const World&);
        World& operator=(const World&);

		bool _dialoguePreGame = false;
		bool _dialoguePreBos = false;
		bool _dialoguePosWin = false;


		bool _dialoguePreGameEnded = false;
		bool _dialoguePreBosEnded = false;
		bool _dialoguePosWinEnded = false;
};

#endif // WORLD_H
