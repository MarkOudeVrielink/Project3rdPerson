#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mgengine\Collision\CollisionManager.h"
#include "mgengine\Core\MeshHolder.h"

#include <string>
class Camera;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
		
		//TODO: make this neat again, you know with a dumb getter.
		CollisionManager* physicsManager;

		void loadMesh(Meshes::ID pId, const std::string pFileName);
		Mesh* getMesh(Meshes::ID pId);

		//void loadMaterial(Materials::ID pId, AbstractMaterial* pMaterial);
		//AbstractMaterial* getMaterial(Materials::ID pId);
	private:
	    Camera* _mainCamera;	

		MeshHolder* _meshHolder;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
