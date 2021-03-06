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

#include "mgengine\Materials\PlayerMaterial.h"

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
#include "mgengine/UI/HUD.h"

#include "mge/config.hpp"
#include "mgengine/Collision/CollisionFilters.h"
#include "mge/MGEDemo.hpp"

#include "btBulletDynamicsCommon.h"

#include "mgengine/Collision/CollisionManager.h"
#include "mgengine/Core/ControlledActor.h"
#include "mgengine/Core/ObjectActor.h"

#include "mygame/Enemy.h"
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mygame/Behaviours/SpaceShipBehaviour.h"
#include "mygame/Behaviours/CameraBehaviour.h"

#include "mgengine/Resources/ResourceHolder.h"
#include "mgengine/Resources/ResourceIdentifiers.h"


sf::Clock timer;
//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo() :AbstractGame(), _debugHud(0), _meshLoadingThread(&MGEDemo::LoadMeshes, this), _materialLoadingThread(&MGEDemo::LoadMaterials, this)
{
	timer.restart().asSeconds();
}

void MGEDemo::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part
	cout << "Initializing HUD" << endl;
	_debugHud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
		
}

//build the game _world
void MGEDemo::_initializeScene()
{	
	_renderer->setClearColor(0, 0, 0);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 100, 0));
	camera->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	_levelEditor = new LevelEditorBehaviour(_window, _world);
	_levelEditor->InitializeHud(&_gui);
	_levelEditor->setActive(false);
		
	_menuScreen = new Menu(_world,_window);

	GameObject *MenuObject = new GameObject("Menu", glm::vec3(0, 0, 0));
	MenuObject->setBehaviour(_menuScreen);
	_world->add(MenuObject);
	_menuScreen->InitializeMenu(&_gui);
		
	LoadMeshes();
	LoadMaterials();
	
#pragma region PlayfieldBoundaries
	Actor* top = new Actor(_world, "top", glm::vec3(0, 0, -55), new btBoxShape(btVector3(50, 5, 1)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	/*top->scale(glm::vec3(40, 2, 2));
	top->setMesh(cube);
	top->setMaterial(colorMaterialGreen);*/
	_world->add(top);

	Actor* bottom = new Actor(_world, "bottom", glm::vec3(0, 0, 55), new btBoxShape(btVector3(50, 5, 1)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//bottom->scale(glm::vec3(40, 2, 2));
	//bottom->setMesh(cube);
	//bottom->setMaterial(colorMaterialGreen);
	_world->add(bottom);

	Actor* left = new Actor(_world, "left", glm::vec3(-45, 0, 0), new btBoxShape(btVector3(1, 5, 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//left->scale(glm::vec3(2, 2, 80));
	//left->setMesh(cube);
	//left->setMaterial(colorMaterialGreen);
	_world->add(left);

	Actor* right = new Actor(_world, "right", glm::vec3(45, 0, 0), new btBoxShape(btVector3(1, 5, 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//right->scale(glm::vec3(2, 2, 80));
	//right->setMesh(cube);
	//right->setMaterial(colorMaterialGreen);
	_world->add(right);
#pragma endregion
		
	std::cout << "============================================" << std::endl;
	std::cout << "Seconds it took to load all assets: " << timer.getElapsedTime().asSeconds() << std::endl;
	std::cout << "============================================" << std::endl;
}

void MGEDemo::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MGEDemo::LoadMeshes()
{
	//_meshLock.lock();
	std::cout << "\n<==================================>" << std::endl;
	std::cout << "Loading models." << std::endl;
	std::cout << "<==================================>\n" << std::endl;
	sf::Clock meshClock;
	
	_world->GetResourceManager()->loadMesh(Meshes::Player, config::MGE_MODEL_PATH + "ship.obj");

	//_world->GetResourceManager()->loadMesh(Meshes::Yogurt, config::MGE_MODEL_PATH + "Yogurt_90.obj");
	//_world->GetResourceManager()->loadMesh(Meshes::Sushi, config::MGE_MODEL_PATH + "Sushi_90.obj");
	//_world->GetResourceManager()->loadMesh(Meshes::Sandwich, config::MGE_MODEL_PATH + "Sandwich_90.obj");
	//_world->GetResourceManager()->loadMesh(Meshes::Potato, config::MGE_MODEL_PATH + "Potato_90.obj");
	//_world->GetResourceManager()->loadMesh(Meshes::Pizza, config::MGE_MODEL_PATH + "Pizza_90.obj");
	//_world->GetResourceManager()->loadMesh(Meshes::Muffin, config::MGE_MODEL_PATH + "Muffin_90.obj");

	_world->GetResourceManager()->loadMesh(Meshes::Yogurt, config::MGE_MODEL_PATH + "yogurt.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Sushi, config::MGE_MODEL_PATH + "sushi.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Sandwich, config::MGE_MODEL_PATH + "sandwich.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Potato, config::MGE_MODEL_PATH + "potato.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Pizza, config::MGE_MODEL_PATH + "pizza.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Muffin, config::MGE_MODEL_PATH + "cupcake.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Boss, config::MGE_MODEL_PATH + "boss.obj");

	_world->GetResourceManager()->loadMesh(Meshes::PickUp, config::MGE_MODEL_PATH + "PickUp(AirFreshner).obj");
	_world->GetResourceManager()->loadMesh(Meshes::Explosion, config::MGE_MODEL_PATH + "plane.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Bullet, config::MGE_MODEL_PATH + "lazer.obj");
	
	_world->GetResourceManager()->loadMesh(Meshes::BackGround, config::MGE_MODEL_PATH + "Backgroundsingle.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Planet, config::MGE_MODEL_PATH + "Planet.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Meteor, config::MGE_MODEL_PATH + "meteor.obj");
	_world->GetResourceManager()->loadMesh(Meshes::MeteorTrail, config::MGE_MODEL_PATH + "meteor_with_trail.obj");

	//_meshLock.unlock();
	std::cout << "<==================================>" << std::endl;
	std::cout << "Loaded models in: " << meshClock.getElapsedTime().asSeconds() << " seconds." << std::endl;
	std::cout << "<==================================>\n" << std::endl;
	
}

void MGEDemo::LoadMaterials()
{
	std::cout << "<==================================>" << std::endl;
	std::cout << "Loading materials." << std::endl;
	std::cout << "<==================================>\n" << std::endl;
	sf::Clock materialClock;

	//_materialLock.lock();

	_world->GetResourceManager()->loadMaterial(Materials::Player, new PlayerMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Yogurt, new EnemyMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Yogurt_Texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Sushi, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sushi_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Sandwich, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Sandwich_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Potato, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Potato_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Pizza, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Pizza_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Muffin, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Muffin_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Boss, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Boss_texture.png")));

	_world->GetResourceManager()->loadMaterial(Materials::PickUp, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "PickUp_airfreshner_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Explosion, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Supermove.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Bullet, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "laser.png")));

	_world->GetResourceManager()->loadMaterial(Materials::BackGround_0, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "FB.png")));
	_world->GetResourceManager()->loadMaterial(Materials::BackGround_1, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "FB1.png")));
	_world->GetResourceManager()->loadMaterial(Materials::BackGround_2, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "FB2.png")));
	_world->GetResourceManager()->loadMaterial(Materials::BackGround_3, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "FB3.png")));

	_world->GetResourceManager()->loadMaterial(Materials::Planet, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Planet_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Meteor, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Meteor_texture.png")));
	_world->GetResourceManager()->loadMaterial(Materials::MeteorTrail, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Meteor_with_trail_texture.png")));

	_world->GetResourceManager()->loadMaterial(Materials::Enemy, new ColorMaterial(glm::vec3(0.2f, 0, 0.5f)));

	//_materialLock.unlock();
	std::cout << "<==================================>" << std::endl;
	std::cout << "Loaded materials in: " << materialClock.getElapsedTime().asSeconds() << " seconds." << std::endl;
	std::cout << "<==================================>\n" << std::endl;
	
}

void MGEDemo::_updateHud() {
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string((int)_fps) + "\n";
	
	_debugHud->setDebugInfo(debugInfo);
	_debugHud->draw();

	if (_levelEditor->getActive())
		_levelEditor->DrawUI();

    _debugHud->setDebugInfo(debugInfo);
    _debugHud->draw();
	
	_menuScreen->UpdateHUD();
	
	_world->getHud()->draw();

}

MGEDemo::~MGEDemo()
{
	//delete collsionManager;
	//dtor
}
