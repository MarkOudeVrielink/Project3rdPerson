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

void Menu::InitializeMenu(tgui::Gui* pGuiRef)
{
	_world->GetResourceManager()->PlayMusic(Music::MenuTheme);
	_world->GetResourceManager()->SetVolume(80.0f);

	_guiRef = pGuiRef;

	auto windowWidth = tgui::bindWidth(*pGuiRef);
	auto windowHeight = tgui::bindHeight(*pGuiRef);
	_panel = tgui::Panel::create();
	_panel->setSize(1920, 1080);
	_panel->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	pGuiRef->add(_panel);

#pragma region Background/title

	/*BackGround*/
	auto backGround = tgui::Button::create();
	backGround->setSize(1920, 1080);
	
	sf::Texture texture;
	texture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/MenuBackground.jpg");
	
	backGround->getRenderer()->setNormalTexture(texture);
	backGround->setPosition(0, 0);
	_panel->add(backGround);

	/*Title*/
	auto titleBanner = tgui::Button::create();
	titleBanner->setSize(650, 500);
	titleBanner->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
	
	sf::Texture title;
	title.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Logo.png");
	
	titleBanner->getRenderer()->setNormalTexture(title);
	titleBanner->setPosition(windowWidth * 0.5f - 370, 0);
	_panel->add(titleBanner);

#pragma endregion

#pragma region Button Setup

	/*StartButton*/
	auto startGameButton = tgui::Button::create();

	startGameButton->setSize(350, 170);
	startGameButton->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
	
	sf::Texture startNormal; 
	sf::Texture startHover;
	sf::Texture startClick;

	//Load the texture.
	startNormal.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Start.png");
	startHover.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/StartHOVER.png");
	startClick.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/StartPRESSED.png");

	//Set right texture to the button events.
	startGameButton->getRenderer()->setNormalTexture(startNormal);
	startGameButton->getRenderer()->setHoverTexture	(startHover);
	startGameButton->getRenderer()->setDownTexture	(startClick);

	//Set the position and add it to the panel.
	startGameButton->setPosition(windowWidth*.5f - 200, windowHeight*.5f );	
	_panel->add(startGameButton);

	/*levelEditorButton*/
	auto LevelEditorButton = tgui::Button::create();	
	
	LevelEditorButton->setSize(260, 140);
	LevelEditorButton->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));

	sf::Texture lvlEditorNormal;
	sf::Texture lvlEditorHover;
	sf::Texture lvlEditorClick;

	lvlEditorNormal.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/LvlEditor.png");
	lvlEditorHover.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/LvlEditorHOVER.png");
	lvlEditorClick.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/LvlEditorPRESSED.png");

	LevelEditorButton->getRenderer()->setNormalTexture(lvlEditorNormal);
	LevelEditorButton->getRenderer()->setHoverTexture(lvlEditorHover);
	LevelEditorButton->getRenderer()->setDownTexture(lvlEditorClick);

	LevelEditorButton->setPosition(windowWidth*.5f - 150, windowHeight*.5f + 200);
	_panel->add(LevelEditorButton);

	/*ExitButton*/
	auto exitButton = tgui::Button::create();
	
	exitButton->setSize(230, 120);
	exitButton->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));  

	sf::Texture exitButtonNormal;
	sf::Texture exitButtonHover;
	sf::Texture exitButtonClick;

	exitButtonNormal.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Exit.png");
	exitButtonHover.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ExitHOVER.png");
	exitButtonClick.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ExitPRESS.png");

	exitButton->getRenderer()->setNormalTexture	(exitButtonNormal);
	exitButton->getRenderer()->setHoverTexture	(exitButtonHover);
	exitButton->getRenderer()->setDownTexture	(exitButtonClick);

	exitButton->setPosition(windowWidth * 0.5f - 135, windowHeight * 0.5f + 350);
	_panel->add(exitButton);

#pragma endregion
	
#pragma region Event Setup

	/*connect the button to the right event.*/
	startGameButton->connect	("pressed", &Menu::StartGame, this);
	LevelEditorButton->connect	("pressed", &Menu::ToLevelEditor, this);
	exitButton->connect			("pressed", &Menu::Exit, this);

	startGameButton->connect	("pressed", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::ButtonSelect, 50.0f); });
	LevelEditorButton->connect	("pressed", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::ButtonSelect, 50.0f); });
	exitButton->connect			("pressed", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::ButtonSelect, 50.0f); });

	startGameButton->connect	("MouseEntered", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::Button, 50.0f); });
	LevelEditorButton->connect	("MouseEntered", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::Button, 50.0f); });
	exitButton->connect			("MouseEntered", [&]() {_world->GetResourceManager()->PlaySound(SoundEffect::Button, 50.0f); });

#pragma endregion
	
}

void Menu::update(float pStep)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		ToMenu();
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

	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith, 3);

	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));

	player->setActorBehaviour(new PlayerBehaviour(1.0f));
	_world->add(player);

	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);*/
}

void Menu::StartGame()
{
	_world->GetResourceManager()->PlayMusic(Music::MissionTheme_1);
	_world->GetResourceManager()->SetVolume(70.0f);

	HideMenu();
	player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith, 3);
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(1.0f));
	_world->add(player);
	_world->setMainPlayer(player);

	_objManager = new GameObject("Manager",glm::vec3(0,0,0));
	 manager = new LevelManager(_world);
	 _objManager->setBehaviour(manager);
	manager->StartGameFromMenu();
	
	_world->add(_objManager);
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
		delete player;
		player = NULL;
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
	_window->close();
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

