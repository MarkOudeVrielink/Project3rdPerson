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
#include "mge/materials/WobblingMaterial.h"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/TerrainMaterial.h"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/OrbitBehaviour.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

#include "mge/Light/LightCache.h"
#include "mge/Light/Light.h"
#include "mge/Light/LightTypes.h"

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
    //_renderer->setClearColor(0,0,1);

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("camera", glm::vec3(0,10,7));
	//camera->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

	LightCache* lightCache = new LightCache();

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault	= Mesh::load (config::MGE_MODEL_PATH +	"plane_8192.obj");
   // Mesh* lightMesh = Mesh::load (config::MGE_MODEL_PATH+"light.obj");
    Mesh* suzannaMeshS		= Mesh::load (config::MGE_MODEL_PATH +	"suzanna_smooth.obj");
    Mesh* teapotMeshS		= Mesh::load (config::MGE_MODEL_PATH +	"teapot_smooth.obj");	
	Mesh* globeMesh			= Mesh::load (config::MGE_MODEL_PATH +	"Sphere_smooth.obj");
		
	AbstractMaterial* colorMaterial = new ColorMaterial(glm::vec3(1, 0.5f, 1));
	AbstractMaterial* colorMaterial1 = new ColorMaterial(glm::vec3(0, 0.5f, 1));
	//// type - name - position - direction - lightColour - ambientColour - Intensity - penumbra/umbra
	Light* light = new Light(LightType::SPOT, "light", glm::vec3(5, 3, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0.5f, 1), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1,0,1), glm::vec2(28, 50));
	light->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	light->setMesh(globeMesh);
	light->setMaterial(colorMaterial);
	light->setBehaviour(new KeysBehaviour(5.0f, 90.0f));
	_world->add(light);
	lightCache->AddLight(light);

	Light* light2 = new Light(LightType::POINT, "light", glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0.5f, 1), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1, 0, 1), glm::vec2(28, 50));
	light2->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	light2->setMesh(globeMesh);
	light2->setMaterial(colorMaterial1);
	light2->setBehaviour(new KeysBehaviour(5.0f, 90.0f));
	_world->add(light2);
	lightCache->AddLight(light2);

    //MATERIALS
    
	//AbstractMaterial* litMaterial = new LitMaterial(light, glm::vec3(0.2f, 0.2f, 0.2f),glm::vec3(0.4f, 0.4f, 0.2f), 256.0f, lightCache);
	//AbstractMaterial* litMaterialPlane = new LitMaterial(light, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1, 0, 0), 32.0f, lightCache);

    AbstractMaterial* textureMaterial	= new TextureMaterial	(	Texture::load (config::MGE_TEXTURE_PATH	+	"land.jpg"));
    AbstractMaterial* textureMaterial2	= new TextureMaterial	(	Texture::load (config::MGE_TEXTURE_PATH	+	"bricks.jpg"));
	AbstractMaterial* terrainMaterial	= new TerrainMaterial	(	Texture::load (config::MGE_TEXTURE_PATH +	"DupeHeightMap.png"),
																	Texture::load (config::MGE_TEXTURE_PATH +	"SplatAlpha.png"),
																	Texture::load (config::MGE_TEXTURE_PATH +	"diffuse3.jpg"),
																	Texture::load (config::MGE_TEXTURE_PATH	+	"diffuse1.jpg"), 
																	Texture::load (config::MGE_TEXTURE_PATH	+	"water.jpg"), 
																	Texture::load (config::MGE_TEXTURE_PATH	+	"diffuse4.jpg"),
																	lightCache, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.2f), 2.0f);
	
    //SCENE SETUP	
    GameObject* plane = new GameObject ("plane", glm::vec3(0,0,0));
	plane->scale(glm::vec3(5,5,5));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(terrainMaterial);
    _world->add(plane);
		
	//lightCache->AddLight(light);
	
	camera->setBehaviour(new OrbitBehaviour(plane, _window, 10, 2.f, 20.f));

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
	//dtor
}
