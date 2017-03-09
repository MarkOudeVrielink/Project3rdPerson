#include "Menu.h"
#include "mygame\LevelManager.h"
#include "SFML\Window.hpp"
#include "mge/core/GameObject.hpp"
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mge/core/World.hpp"

Menu::Menu(World* pWolrd, sf::RenderWindow *pWindow)
{
	_window = pWindow;
	_world = pWolrd;
	//InitializeMenu();



}
Menu::~Menu()
{
	if (_levelEditorObject != NULL &&_levelEditor->getActive())
	{
		_levelEditor->setActive(false);
		delete _levelEditorObject;
		_levelEditorObject = NULL;
	}
	if (_objManager != NULL)
	{
		delete _objManager;
		_objManager = NULL;
		if (player)
			delete player;
		player = NULL;
		_scoreLabel->hide();
		_multiplierLabel->hide();
		_nextLevel->hide();
	}
}
void Menu::InitializeMenu(tgui::Gui* pGuiRef)
{
	_guiRef = pGuiRef;

	auto windowWidth = tgui::bindWidth(*pGuiRef);
	auto windowHeight = tgui::bindHeight(*pGuiRef);
	_panel = tgui::Panel::create();
	_panel->setSize(1920, 1080);
	_panel->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	pGuiRef->add(_panel);

	auto startGameButton = tgui::Button::create();
	startGameButton->setText("Start Game");
	//startGameButton->
	startGameButton->setSize(300, 100);
	startGameButton->setPosition(windowWidth*.5f - 150, windowHeight*.5f - 50);
	_panel->add(startGameButton);

	auto LevelEditorButton = tgui::Button::copy(startGameButton);
	LevelEditorButton->setText("Level Editor");
	LevelEditorButton->setSize(250, 100);
	LevelEditorButton->setPosition(windowWidth*.5f - 125, windowHeight*.6f - 50);
	_panel->add(LevelEditorButton);

	startGameButton->connect("pressed", &Menu::StartGame, this);
	LevelEditorButton->connect("pressed", &Menu::ToLevelEditor, this);

}

void Menu::update(float pStep)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		ToMenu();
	}
	if (_objManager != NULL)
	{
		PlayerBehaviour * playerBehaviour = dynamic_cast<PlayerBehaviour*> (player->getActorBehaviour());
		_scoreLabel->setText("Score: " + std::to_string((int)playerBehaviour->getScore()));
		_multiplierLabel->setText("Multiplier X " + std::to_string(playerBehaviour->getMultiplier()));

		int score = playerBehaviour->getScore();
		if (score < 500)
			_nextLevel->setText(" Score To Next Level: 500");
		else if (score < 1000)
			_nextLevel->setText(" Score To Next Level: 1000");
		else if (score >= 1000)
			_nextLevel->setText(" MAX LEVEL");

		
	}
}

void Menu::ToLevelEditor()
{
	HideMenu();
	_levelEditorObject = new GameObject("LevelEditor", glm::vec3(0, 0, 0));
	_levelEditor = new LevelEditorBehaviour(_window, _world);	
	_levelEditorObject->setBehaviour(_levelEditor);
	_levelEditor->InitializeHud(_guiRef);
	_world->add(_levelEditorObject);
	_levelEditor->setActive(true);
	/*
	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith,3);
=======

	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith, 3);
>>>>>>> origin/master
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);*/


}
void Menu::SetScoreHUD()
{

	auto theme = tgui::Theme::create("TGUI-0.7/widgets/TransparentGrey.txt");
	_scoreLabel = theme->load("label");
	PlayerBehaviour * playerBehaviour = dynamic_cast<PlayerBehaviour*> (player->getActorBehaviour());
	_scoreLabel->setText("Score: " + std::to_string((int)playerBehaviour->getScore()));
	_scoreLabel->setPosition(350, 100);
	_scoreLabel->setTextSize(28);
	_guiRef->add(_scoreLabel);	


	_multiplierLabel = theme->load("label");
	_multiplierLabel->setText("Multiplier X " + std::to_string(playerBehaviour->getMultiplier()));
	_multiplierLabel->setPosition(350, 150);
	_multiplierLabel->setTextSize(28);
	_guiRef->add(_multiplierLabel);

	_nextLevel = theme->load("label");
	_nextLevel->setText(" Score To Next Level: 500" );
	_nextLevel->setPosition(350, 50);
	_nextLevel->setTextSize(28);
	_guiRef->add(_nextLevel);
}
void Menu::StartGame()
{
	HideMenu();
	player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith, 3);
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);
	_world->setMainPlayer(player);
	_objManager = new GameObject("Manager",glm::vec3(0,0,0));
	 manager = new LevelManager(_world);
	 _objManager->setBehaviour(manager);
	manager->StartGameFromMenu();
	
	_world->add(_objManager);
	SetScoreHUD();
}

void Menu::HideMenu()
{
	setActive(false);
}

void Menu::ToMenu()
{
	if (_levelEditorObject != NULL &&_levelEditor->getActive() )
	{
		_levelEditor->setActive(false);
		delete _levelEditorObject;
		_levelEditorObject = NULL;
	}
	if (_objManager != NULL)
	{
		delete _objManager;
		_objManager = NULL;
		if(player)
		delete player;
		player = NULL;
		_scoreLabel->hide();
		_nextLevel->hide();
		_multiplierLabel->hide();
	}
	setActive(true);
	
}
void Menu::UpdateHUD()
{
	if (_levelEditorObject!= NULL)
		_levelEditor->DrawUI();
}
void Menu::Exit()
{
}
void Menu::setActive(bool pActive)
{
	_active = pActive;
	if(_panel)
	if (!_active)
		_panel->hide();
	else
		_panel->show();
}

bool Menu::getActive()
{
	return _active;
}

