#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace Materials {
	enum ID {
		Player,
		Enemy,
		Bullet
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
	void load(Identifier id, Resource pResource);
	template<typename Parameter>
	void load(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam);

	Resource&		get(Identifier pId);
	const Resource& get(Identifier pId) const;

private:
	
	std::map<Identifier, std::unique_ptr<Resource>> _resourceMap;
};

#pragma region inline

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier pId, Resource pResource) {
	std::unique_ptr<Resource> resource(pResource);
	
	auto inserted = _resourceMap.insert(std::make_pair(pId, std::move(resource)));
	_ASSERT(inserted.second);
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam) {
	std::unique_ptr<Resource> resource(new Resource());

	auto inserted = _resourceMap.insert(std::make_pair(pId, std::move(resource)));
	_ASSERT(inserted.second);
}

template <typename Resource, typename Identifier>
Resource&::ResourceHolder<Resource, Identifier>::get(Identifier pId) {
	auto found = _resourceMap.find(pId);
	_ASSERT(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource&::ResourceHolder<Resource, Identifier>::get(Identifier pId) const {
	auto found = _resourceMap.find(pId);
	_ASSERT(found != _resourceMap.end());

	return *found->second;
}
#pragma endregion

#endif // !RESOURCEHOLDER_H

