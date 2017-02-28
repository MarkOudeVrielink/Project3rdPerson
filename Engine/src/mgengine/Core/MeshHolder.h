#ifndef MESHHOLDER_H
#define MESHHOLDER_H

#include <memory>
#include <map>

namespace Meshes {
	enum ID {
		Player,
		Enemy,
		Bullet
	};
}

class Mesh;

class MeshHolder {
public:
	MeshHolder();
	~MeshHolder();
	void Load(Meshes::ID pId, const std::string pFileName);
	Mesh* Get(Meshes::ID pId);
private:
	std::map<Meshes::ID, Mesh*> _meshMap;	
};

#endif // !MESHHOLDER_H

