#include "mgengine\Resources\ResourceManager.h"

#include "mge\materials\AbstractMaterial.hpp"
#include "mge\core\Mesh.hpp"

#include <iostream>
ResourceManager::ResourceManager() : _soundPlayer(), _musicPlayer(), _resourceHolder()
{			
}

ResourceManager::~ResourceManager()
{
}

#pragma region Mesh/textures

void ResourceManager::loadMesh(Meshes::ID pId, const std::string pFileName)
{
	_resourceHolder.LoadMesh(pId, pFileName);	
}

Mesh* ResourceManager::getMesh(Meshes::ID pId)
{
	return _resourceHolder.GetMesh(pId);
}

void ResourceManager::loadMaterial(Materials::ID pId, AbstractMaterial* pMaterial)
{
	_resourceHolder.LoadMaterial(pId, pMaterial);
}

AbstractMaterial* ResourceManager::getMaterial(Materials::ID pId)
{
	return _resourceHolder.GetMaterial(pId);
}

#pragma endregion

#pragma region Audio

void ResourceManager::PlaySound(SoundEffect::ID pEffect)
{
	_soundPlayer.Play(pEffect);
}

void ResourceManager::PlayMusic(Music::ID pMusic)
{
	_musicPlayer.Play(pMusic);
}

void ResourceManager::StopMusic()
{
	_musicPlayer.Stop();
}

void ResourceManager::PauseMusic(bool pPause)
{
	_musicPlayer.SetPaused(pPause);
}

void ResourceManager::SetVolume(float pVolume)
{
	_musicPlayer.SetVolume(pVolume);
}
#pragma endregion


