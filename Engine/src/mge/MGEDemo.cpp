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

#include "mgengine/UI/ImageBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

#include "btBulletCollisionCommon.h"

#include "mygame/Enemy.h"
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mygame\Behaviours\SpaceShipBehaviour.h"
#include "mygame\Behaviours\CameraBehaviour.h"


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

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    //Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* testingMesh = Mesh::load (config::MGE_MODEL_PATH+"explosion.obj");
    Mesh* teapotMeshS = Mesh::load (config::MGE_MODEL_PATH+"ship.obj");

    //MATERIALS

    AbstractMaterial* colorMaterial = new ColorMaterial (glm::vec3(0.2f,0,0.2f));
    AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"land.jpg"));
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "ship.png"));
	AbstractMaterial* textureTestingObject = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "explosion.png"));

    //SCENE SETUP

    GameObject* plane = new GameObject ("plane", glm::vec3(0,0,0));
    plane->scale(glm::vec3(5,5,5));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(textureMaterial);
    //_world->add(plane);

	Enemy* Enemy1 = new Enemy("Diablo", glm::vec3(0, 0, 0));
	Enemy1->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	//Enemy1->scale(glm::vec3(.5f, .5f, .5f));
	Enemy1->setMesh(teapotMeshS);
	Enemy1->setMaterial(textureMaterial2);
	Enemy1->setBehaviour(new SpaceShipBehaviour(60, 60));
	_world->add(Enemy1);


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

   // camera->setBehaviour(new LookAt (teapot));
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
	//dtor
}
