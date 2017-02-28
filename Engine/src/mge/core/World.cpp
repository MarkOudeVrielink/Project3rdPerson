#include "mge/core/World.hpp"

using namespace std;

World::World():GameObject("root"), _mainCamera(0)
{
	//ctor
	physicsManager = new CollisionManager();
	_meshHolder = new MeshHolder();
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

void World::loadMesh(Meshes::ID pId, const std::string pFileName)
{
	_meshHolder->Load(pId, pFileName);
}

Mesh* World::getMesh(Meshes::ID pId)
{	
	return _meshHolder->Get(pId);
}

//void World::loadMaterial(Materials::ID pId, AbstractMaterial* pMaterial)
//{
//	_materialHolder.load(pId, pMaterial);
//}

//AbstractMaterial * World::getMaterial(Materials::ID pId)
//{
//	return _materialHolder.get(pId);
//}
