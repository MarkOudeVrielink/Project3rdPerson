#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mgengine/Materials/PlayerMaterial.h"
#include "mgengine/Materials/EnemyMaterial.h"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge\behaviours\OrbitBehaviour.h"

#include "mgengine\Behaviours\PlayerBehaviour.h"
#include "mygame\Behaviours\EnemyBehaviour.h"
#include "mgengine\Behaviours\ActorEnemyBehaviour.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"

#include "mgengine/UI/ImageBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mgengine\Collision\CollisionFilters.h"
#include "mge/MGEDemo.hpp"

#include "btBulletDynamicsCommon.h"

#include "mgengine\Collision\CollisionManager.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Core\ObjectActor.h"

#include "mygame/Enemy.h"
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mygame\Behaviours\SpaceShipBehaviour.h"
#include "mygame\Behaviours\CameraBehaviour.h"

#include "mgengine\Resources\ResourceHolder.h"
#include "mgengine\Resources\ResourceIdentifiers.h"

#include "mygame\Menu.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
    _renderer->setClearColor(0,0,0);	
    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,100,0));
    camera->rotate(glm::radians(-90.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

	_levelEditor = new LevelEditorBehaviour(_window, _world);
	_levelEditor->InitializeHud(&_gui);
	_levelEditor->setActive(false);
	
	
	Menu *menuScreen = new Menu(_world, _levelEditor);
	GameObject *MenuObject = new GameObject("Menu", glm::vec3(0, 0, 0));
	MenuObject->setBehaviour(menuScreen);
	_world->add(MenuObject);

	menuScreen->InitializeMenu(&_gui);
	//MESHES
	_world->GetResourceManager()->loadMesh(Meshes::Player,		config::MGE_MODEL_PATH + "ship.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Yogurt,		config::MGE_MODEL_PATH + "Yogurt_90.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Sushi,		config::MGE_MODEL_PATH + "Sushi_90.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Sandwich,	config::MGE_MODEL_PATH + "Sandwich_90.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Potato,		config::MGE_MODEL_PATH + "Potato_90.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Pizza,		config::MGE_MODEL_PATH + "Pizza_90.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Muffin,		config::MGE_MODEL_PATH + "Muffin_90.obj");

	_world->GetResourceManager()->loadMesh(Meshes::PickUp,		config::MGE_MODEL_PATH + "PickUp(AirFreshner).obj");
	_world->GetResourceManager()->loadMesh(Meshes::Explosion,	config::MGE_MODEL_PATH + "Explosion.obj");

	_world->GetResourceManager()->loadMesh(Meshes::Bullet,		config::MGE_MODEL_PATH + "lazer.obj");//change mesh file
	_world->GetResourceManager()->loadMaterial(Materials::Bullet, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "laser.png")));

	
    //MATERIALS
	_world->GetResourceManager()->loadMaterial(Materials::Player,		new PlayerMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "ship.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Yogurt,		new EnemyMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Yogurt_Texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Sushi,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Sushi_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Sandwich,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Sandwich_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Potato,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Potato_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Pizza,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Pizza_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Muffin,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "Muffin_texture.png")));

	_world->GetResourceManager()->loadMaterial(Materials::PickUp,		new TextureMaterial	(Texture::load(config::MGE_TEXTURE_PATH + "PickUp_airfreshner_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Explosion,	new TextureMaterial (Texture::load(config::MGE_TEXTURE_PATH + "Explosion_Texture.png")));

	_world->GetResourceManager()->loadMaterial(Materials::Enemy,		new ColorMaterial (glm::vec3(0.2f,0,0.5f)));


	_world->GetResourceManager()->PlayMusic(Music::MenuTheme);
	_world->GetResourceManager()->SetVolume(30.0f);
	
#pragma region PlayfieldBoundaries
	Actor* top = new Actor(_world, "top", glm::vec3(0,0,-55), new btBoxShape(btVector3(50, 1, 1)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	/*top->scale(glm::vec3(40, 2, 2));
	top->setMesh(cube);
	top->setMaterial(colorMaterialGreen);*/
	_world->add(top);

	Actor* bottom = new Actor(_world, "bottom", glm::vec3(0, 0, 55), new btBoxShape(btVector3(50, 1, 1)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//bottom->scale(glm::vec3(40, 2, 2));
	//bottom->setMesh(cube);
	//bottom->setMaterial(colorMaterialGreen);
	_world->add(bottom);

	Actor* left = new Actor(_world, "left", glm::vec3(-45, 0, 0), new btBoxShape(btVector3(1, 1, 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//left->scale(glm::vec3(2, 2, 80));
	//left->setMesh(cube);
	//left->setMaterial(colorMaterialGreen);
	_world->add(left);

	Actor* right = new Actor(_world, "right", glm::vec3(45, 0, 0), new btBoxShape(btVector3(1, 1 , 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//right->scale(glm::vec3(2, 2, 80));
	//right->setMesh(cube);
	//right->setMaterial(colorMaterialGreen);
	_world->add(right);
#pragma endregion

	//camera->setBehaviour(new OrbitBehaviour(player, _window, 15, 5, 15));	
}

void MGEDemo::_render() {
   AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
	if(_levelEditor->getActive())
	_levelEditor->DrawUI();
}

MGEDemo::~MGEDemo()
{
	//delete collsionManager;
	//dtor
}
