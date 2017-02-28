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
	//camera->setBehaviour(new CameraBehaviour(20));
   
	//MESHES	  
    //Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    //Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
	//_world->loadMesh(Meshes::Player, Mesh::load(config::MGE_MODEL_PATH + "ship.obj"));
    Mesh* testingMesh = Mesh::load (config::MGE_MODEL_PATH+"explosion.obj");
    Mesh* teapotMeshS = Mesh::load (config::MGE_MODEL_PATH+"ship.obj");

    //MATERIALS
    //AbstractMaterial* colorMaterial0 = new ColorMaterial (glm::vec3(0.2f,0,0.2f));
	//AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"land.jpg"));
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png"));
	AbstractMaterial* textureTestingObject = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "explosion.png"));

    //SCENE SETUP  
	/**/
	//Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
	//Enemy1->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	////Enemy1->scale(glm::vec3(.5f, .5f, .5f));
	//Enemy1->setMesh(teapotMeshS);
	//Enemy1->setMaterial(textureMaterial2);
	//Enemy1->setBehaviour(new SpaceShipBehaviour(60, 60));
	//_world->add(Enemy1);
	
	///Enemy1->add(camera);
    /*GameObject* spaceShip = new GameObject ("teapot", glm::vec3(-3,1,0));
	spaceShip->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	spaceShip->scale(glm::vec3(.01f, .01f, .01f));

    spaceShip->setMesh (teapotMeshS);
    spaceShip->setMaterial(textureMaterial2);
    spaceShip->setBehaviour (new KeysBehaviour(600,600));
    _world->add(spaceShip);
	*/
	if (false)
	{
		for (int i = 0; i < 1;i++)
		{
			GameObject* testingModel = new GameObject("Testing Quick Object", glm::vec3(0, 1, -10 * i));
			testingModel->setMesh(testingMesh);
			testingModel->scale(glm::vec3(5.0f, 5.0f, 5.0f));
			testingModel->setMaterial(textureTestingObject);
			testingModel->setBehaviour(new RotatingBehaviour());
			_world->add(testingModel);
		}
	}
  
	
	_levelEditor = new LevelEditorBehaviour(_window,_world);
	GameObject *LevelEditor = new GameObject("test", glm::vec3(0, 0, 0));
	LevelEditor->setBehaviour(_levelEditor);
	_world->add(LevelEditor);

	_levelEditor->InitializeHud(&_gui);

	/**/

   // camera->setBehaviour(new LookAt (teapot));

    /*Camera* camera = new Camera ("camera", glm::vec3(0,30,0));
    camera->rotate(glm::radians(-90.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);
	*/

    //MESHES
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");  
	Mesh* cube = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	_world->GetResourceManager()->loadMesh(Meshes::Player, config::MGE_MODEL_PATH + "ship.obj");
	
    //MATERIALS
	_world->GetResourceManager()->loadMaterial(Materials::Player, new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png")));
	_world->GetResourceManager()->loadMaterial(Materials::Enemy, new ColorMaterial (glm::vec3(0.2f,0,0.5f)));

	AbstractMaterial* colorMaterialGreen = new ColorMaterial(glm::vec3(0.0f, 1, 0.0f));
    AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"engin1.png"));
  		
	ControlledActor* player = new ControlledActor(_world, "Player", glm::vec3(0, 0, 3), new btSphereShape(1), ActorType::Type_Player, 1, CF::COL_PLAYER, CF::playerCollidesWith);	
	player->scale(glm::vec3(0.8f, 0.8f, 0.8f));
	player->setMesh(_world->GetResourceManager()->getMesh(Meshes::Player));
	player->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Player));
	player->setActorBehaviour(new PlayerBehaviour(_world->GetResourceManager()->getMesh(Meshes::Player), _world->GetResourceManager()->getMaterial(Materials::Player), 20));
	_world->add(player);	

	ControlledActor* enemy0 = new ControlledActor(_world, "ENEMY0", glm::vec3(-20, 0, -18), new btSphereShape(1), ActorType::Type_Enemy, 15, CF::COL_ENEMY, CF::enemyCollidesWith, 3);
	enemy0->setMesh(teapotMeshS);
	enemy0->setMaterial(_world->GetResourceManager()->getMaterial(Materials::Enemy));
	enemy0->setActorBehaviour(new ActorEnemyBehaviour(teapotMeshS, colorMaterialGreen));
	_world->add(enemy0);	

	//_world->GetResourceManager()->PlayMusic(Music::MenuTheme);
	
#pragma region PlayfieldBoundaries
	Actor* top = new Actor(_world, "top", glm::vec3(0,0,-55), new btBoxShape(btVector3(50, 0.5f, 0.5f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//top->scale(glm::vec3(40, 1, 1));
	//top->setMesh(cube);
	//top->setMaterial(colorMaterialGreen);
	_world->add(top);

	Actor* bottom = new Actor(_world, "bottom", glm::vec3(0, 0, 55), new btBoxShape(btVector3(50, 0.5f, 0.5f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//bottom->scale(glm::vec3(40, 1, 1));
	//bottom->setMesh(cube);
	//bottom->setMaterial(colorMaterialGreen);
	_world->add(bottom);

	Actor* left = new Actor(_world, "left", glm::vec3(-45, 0, 0), new btBoxShape(btVector3(0.5f, 0.5f, 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//left->scale(glm::vec3(1, 1, 80));
	//left->setMesh(cube);
	//left->setMaterial(colorMaterialGreen);
	_world->add(left);

	Actor* right = new Actor(_world, "right", glm::vec3(45, 0, 0), new btBoxShape(btVector3(0.5f, 0.5f, 80.0f)), ActorType::Type_StaticObject, CF::COL_BOUNDARY, CF::boundaryCollidesWith, 0);
	//right->scale(glm::vec3(1, 1, 80));
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
	_levelEditor->DrawUI();
}

MGEDemo::~MGEDemo()
{
	//delete collsionManager;
	//dtor
}
