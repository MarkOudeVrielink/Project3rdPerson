#include "mgengine\Core\MeshHolder.h"
#include "mge\core\Mesh.hpp"

MeshHolder::MeshHolder()
{
	
}

MeshHolder::~MeshHolder()
{
}

void MeshHolder::Load(Meshes::ID pId, const std::string pFileName) {
	Mesh* mesh = Mesh::load(pFileName);
	auto inserted = _meshMap.insert(std::make_pair(pId, std::move(mesh)));
	_ASSERT(inserted.second);
}

Mesh* MeshHolder::Get(Meshes::ID pId) {
	auto found = _meshMap.find(pId);
	_ASSERT(found != _meshMap.end());

	return found->second;
}
