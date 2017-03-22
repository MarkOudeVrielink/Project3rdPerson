#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Resources/ResourceManager.h"

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

		void DestroyActors();
		void setBossDeath(bool pState);
		bool getBossDeath();
		void SetDirtyActor(Actor* pActor);

		
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
