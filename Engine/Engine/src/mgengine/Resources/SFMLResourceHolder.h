#ifndef SFMLRESOURCEHOLDER_H
#define SFMLRESOURCEHOLDER_H

#include <SFML/Graphics.hpp>
#include <memory>

template <typename Resource, typename Identifier>
class SFMLResourceHolder {
public:
	void load(Identifier id, const std::string& filename);
	template<typename Parameter>
	void load(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam);

	Resource&		get(Identifier pId);
	const Resource& get(Identifier pId) const;

private:
	std::map<Identifier, std::unique_ptr<Resource>> _resourceMap;
};

#pragma region inline

template <typename Resource, typename Identifier>
void SFMLResourceHolder<Resource, Identifier>::load(Identifier pId, const std::string& pFilename) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(pFilename)) {
		throw std::runtime_error("ResourceHolder::load - Failed to load " + pFilename);
	}

	auto inserted = _resourceMap.insert(std::make_pair(pId, std::move(resource)));
	_ASSERT(inserted.second);
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void SFMLResourceHolder<Resource, Identifier>::load(Identifier pId, const std::string& pFilename, const Parameter& pSecondParam) {
	std::unique_ptr<Resource> resource(new Resource());

	if (!resource->loadFromFile(pFilename, pSecondParam)) {
		std::runtime_error("ResourceHolder::load - Failed to load: " + pFilename);
	}

	auto inserted = _resourceMap.insert(std::make_pair(pId, std::move(resource)));
	_ASSERT(inserted.second);
}

template <typename Resource, typename Identifier>
Resource&::SFMLResourceHolder<Resource, Identifier>::get(Identifier pId) {
	auto found = _resourceMap.find(pId);
	_ASSERT(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource&::SFMLResourceHolder<Resource, Identifier>::get(Identifier pId) const {
	auto found = _resourceMap.find(pId);
	_ASSERT(found != _resourceMap.end());

	return *found->second;
}
#pragma endregion

#endif // !SFMLRESOURCEHOLDER_H

