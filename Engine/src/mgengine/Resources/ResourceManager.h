#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "mgengine\Resources\ResourceIdentifiers.h"
#include "mgengine\Resources\MusicPlayer.h"
#include "mgengine\Resources\SoundPlayer.h"
#include "mgengine\Resources\ResourceHolder.h"

class Mesh;
class AbstractMaterial;

class ResourceManager {
public:
		ResourceManager();
		~ResourceManager();

		void loadMesh(Meshes::ID pId, const std::string pFileName);
		Mesh* getMesh(Meshes::ID pId);

		void loadMaterial(Materials::ID pId, AbstractMaterial* pMaterial);
		AbstractMaterial* getMaterial(Materials::ID pId);

		void PlaySound(SoundEffect::ID pEffect);

		void PlayMusic(Music::ID pMusic);
		void StopMusic();
		void PauseMusic(bool pPause);
		void SetVolume(float pVolume);		
private:

	MusicPlayer		_musicPlayer;
	SoundPlayer		_soundPlayer;
	ResourceHolder	_resourceHolder;
};

#endif // !RESOURCEMANAGER_H

