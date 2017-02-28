#include "mgengine\Resources\ResourceHolder.h"
#include "mge\core\Mesh.hpp"

ResourceHolder::ResourceHolder()
{
	
}

ResourceHolder::~ResourceHolder()
{
}

void ResourceHolder::LoadMesh(Meshes::ID pId, const std::string pFileName) {
	Mesh* mesh = Mesh::load(pFileName);
	auto inserted = _meshMap.insert(std::make_pair(pId, std::move(mesh)));
	_ASSERT(inserted.second);
}

Mesh* ResourceHolder::GetMesh(Meshes::ID pId) {
	auto found = _meshMap.find(pId);
	_ASSERT(found != _meshMap.end());

	return found->second;
}

void ResourceHolder::LoadMaterial(Materials::ID pId, AbstractMaterial* pMaterial)
{	
	auto inserted = _materialMap.insert(std::make_pair(pId, std::move(pMaterial)));
	_ASSERT(inserted.second);
}

AbstractMaterial* ResourceHolder::GetMaterial(Materials::ID pId)
{
	auto found = _materialMap.find(pId);
	_ASSERT(found != _materialMap.end());

	return found->second;;
}
