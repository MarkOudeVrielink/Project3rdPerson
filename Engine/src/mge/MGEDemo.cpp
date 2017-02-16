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

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge\behaviours\OrbitBehaviour.h"

#include "mgengine\Behaviours\PlayerBehaviour.h"
#include "mgengine\Behaviours\EnemyBehaviour.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mgengine\Collision\CollisionFilters.h"
#include "mge/MGEDemo.hpp"

#include "btBulletDynamicsCommon.h"

#include "mgengine\Collision\CollisionManager.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Core\ObjectActor.h"

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
    Camera* camera = new Camera ("camera", glm::vec3(0,30,0));
    camera->rotate(glm::radians(-90.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

    //#MESHES
    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");    
    Mesh* suzannaMeshF = Mesh::load (config::MGE_MODEL_PATH+"suzanna_flat.obj");
    Mesh* teapotMeshS = Mesh::load (config::MGE_MODEL_PATH+"teapot_smooth.obj");

    //#MATERIALS
    AbstractMaterial* colorMaterial = new ColorMaterial (glm::vec3(0.2f,0,0.5f));
	AbstractMaterial* colorMaterialGreen = new ColorMaterial(glm::vec3(0.0f, 1, 0.0f));
    AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"engin1.png"));
    AbstractMaterial* textureMaterial2 = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"));

	GameObject* plane = new GameObject("plane", glm::vec3(0,0,0));
	plane->scale(glm::vec3(5,0,5));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	//_world->add(plane);	

	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith);	
	player->setMesh(suzannaMeshF);
	player->setMaterial(colorMaterial);	
	player->setActorBehaviour(new PlayerBehaviour(suzannaMeshF, colorMaterial, 20));
	_world->add(player);	

	ControlledActor* enemy0 = new ControlledActor(_world, "ENEMY0", glm::vec3(-20, 0, -18), new btSphereShape(1), ActorType::Type_Enemy, 15, CF::COL_ENEMY, CF::enemyCollidesWith, 3);
	enemy0->setMesh(teapotMeshS);
	enemy0->setMaterial(colorMaterial);
	enemy0->setActorBehaviour(new EnemyBehaviour(teapotMeshS, colorMaterialGreen));
	_world->add(enemy0);	

	ControlledActor* enemy1 = new ControlledActor(_world, "ENEMY1", glm::vec3(0, 0, -15), new btSphereShape(1), ActorType::Type_Enemy, 15, CF::COL_ENEMY, CF::enemyCollidesWith, 3);
	enemy1->setMesh(teapotMeshS);
	enemy1->setMaterial(colorMaterial);
	enemy1->setActorBehaviour(new EnemyBehaviour(teapotMeshS, colorMaterialGreen));
	_world->add(enemy1);

	ControlledActor* enemy2 = new ControlledActor(_world, "ENEMY2", glm::vec3(20, 0, -1), new btSphereShape(1), ActorType::Type_Enemy, 15, CF::COL_ENEMY, CF::enemyCollidesWith, 3);
	enemy2->setMesh(teapotMeshS);
	enemy2->setMaterial(colorMaterial);
	enemy2->setActorBehaviour(new EnemyBehaviour(teapotMeshS, colorMaterialGreen));
	_world->add(enemy2);
	
	ObjectActor* pickUp = new ObjectActor(_world, "PickUp", glm::vec3(-15, 0, -5), new btSphereShape(0.5f), ActorType::Type_PickUp, CF::COL_PICKUP, CF::pickupCollidesWith, 1);
	pickUp->setMesh(teapotMeshS);
	pickUp->setMaterial(colorMaterialGreen);
	pickUp->setActorBehaviour(new PickUpBehaviour());
	_world->add(pickUp);
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
}

MGEDemo::~MGEDemo()
{
	//delete collsionManager;
	//dtor
}
