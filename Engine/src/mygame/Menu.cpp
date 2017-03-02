#include "Menu.h"
#include "mygame\LevelManager.h"
#include "SFML\Window.hpp"
#include "mge/core/GameObject.hpp"
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "mge/core/World.hpp"

Menu::Menu(World* pWolrd, LevelEditorBehaviour * pLevelEditor)
{
	
	_world = pWolrd;
	//InitializeMenu();
	_levelEditor = pLevelEditor;
	

}

void Menu::InitializeMenu(tgui::Gui* pGuiRef)
{
	auto windowWidth = tgui::bindWidth(*pGuiRef);
	auto windowHeight = tgui::bindHeight(*pGuiRef);
	_panel = tgui::Panel::create();
	_panel->setSize(1920, 1080);
	_panel->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	pGuiRef->add(_panel);

	auto startGameButton = tgui::Button::create();
	startGameButton->setText("Start Game");
	startGameButton->setSize(300, 100);
	startGameButton->setPosition(windowWidth*.5f-150, windowHeight*.5f-50);
	_panel->add(startGameButton);

	auto LevelEditorButton = tgui::Button::copy(startGameButton);
	LevelEditorButton->setText("Level Editor");
	LevelEditorButton->setSize(250, 100);
	LevelEditorButton->setPosition(windowWidth*.5f - 125, windowHeight*.6f-50);
	_panel->add(LevelEditorButton);

	startGameButton->connect("pressed", &Menu::StartGame, this);
	LevelEditorButton->connect("pressed", &Menu::ToLevelEditor, this);
}

void Menu::update(float pStep)
{
}

void Menu::ToLevelEditor()
{
	HideMenu();
	GameObject *LevelEditor = new GameObject("test", glm::vec3(0, 0, 0));
	LevelEditor->setBehaviour(_levelEditor);
	_world->add(LevelEditor);
	_levelEditor->setActive(true);

	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith);
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);


}

void Menu::StartGame()
{
	HideMenu();
	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith);
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);


	GameObject *ObjManager = new GameObject("Manager");
	LevelManager * manager = new LevelManager(_world);
	manager->StartGameFromMenu();
	ObjManager->setBehaviour(manager);
	_world->add(ObjManager );
}

void Menu::HideMenu()
{
	setActive(false);
}

void Menu::Exit()
{
}
void Menu::setActive(bool pActive)
{
	_active = pActive;
	if (!_active)
		_panel->hide();
	else
		_panel->show();
}

bool Menu::getActive()
{
	return _active;
}

