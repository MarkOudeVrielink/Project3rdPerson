#ifndef MGEDEMO_H
#define MGEDEMO_H

#include <mge/core/AbstractGame.hpp>
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mygame\Menu.h"
class DebugHud;
class HUD;

class MGEDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();

        virtual void initialize();

	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud*	_debugHud;//hud display	
		

		sf::Thread _meshLoadingThread;
		sf::Thread _materialLoadingThread;

		sf::Mutex _meshLock;
		sf::Mutex _materialLock;

		void LoadMeshes();
		void LoadMaterials();

        void _updateHud();

        MGEDemo(const MGEDemo&);
        MGEDemo& operator=(const MGEDemo&);
		LevelEditorBehaviour * _levelEditor;
		Menu * _menuScreen;
};

#endif // MGEDEMO_H
