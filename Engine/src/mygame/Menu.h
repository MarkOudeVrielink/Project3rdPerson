#pragma once
#include <TGUI/TGUI.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/World.hpp"

#include "mgengine\Collision\CollisionManager.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Core\ObjectActor.h"
#include "mygame\LevelManager.h"
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mgengine\Behaviours\PlayerBehaviour.h"
#include "mgengine\Collision\CollisionFilters.h"
using namespace std;
class Menu :public AbstractBehaviour
{
public:
	Menu(World * pWolrd, sf::RenderWindow * pWindow);
	~Menu();
	void InitializeMenu(tgui::Gui * pGuiRef);
	virtual void update(float pStep);
	void UpdateHUD();
private:
	void ToLevelEditor();
	void SetScoreHUD();
	void StartGame();
	void HideMenu();
	void ToMenu();	
	void Exit();
	void setActive(bool pActive);
	bool getActive();
	bool _active;
	World* _world;
	tgui::Panel::Ptr _panel;
	tgui::Label::Ptr _scoreLabel;
	tgui::Label::Ptr _nextLevel;
	tgui::Label::Ptr _multiplierLabel;
	GameObject *_levelEditorObject = NULL;
	LevelEditorBehaviour * _levelEditor;
	sf::RenderWindow* _window;
	GameObject *_objManager = NULL;
	LevelManager * manager;
	tgui::Gui* _guiRef;
	ControlledActor* player;
	
	tgui::Button::Ptr _titleBanner;
	tgui::Button::Ptr _titleBanner2;

	void _flickerLight();
	int _flicker = 0;
};
