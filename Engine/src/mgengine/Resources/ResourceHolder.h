#ifndef MESHHOLDER_H
#define MESHHOLDER_H

#include <memory>
#include <map>

#include"mgengine\Resources\ResourceIdentifiers.h"

class Mesh;
class AbstractMaterial;

class ResourceHolder {
public:
	ResourceHolder();
	~ResourceHolder();

	void	LoadMesh(Meshes::ID pId, const std::string pFileName);
	Mesh*	GetMesh(Meshes::ID pId);

	void				LoadMaterial(Materials::ID pId, AbstractMaterial* pMaterial);
	AbstractMaterial*	GetMaterial(Materials::ID pId);
private:
	std::map<Meshes::ID, Mesh*>					_meshMap;
	std::map<Materials::ID, AbstractMaterial*>	_materialMap;
};

#endif // !MESHHOLDER_H

