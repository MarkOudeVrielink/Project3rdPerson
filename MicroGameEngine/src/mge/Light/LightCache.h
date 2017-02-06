#ifndef LIGHTCACHE_H
#define LIGHTCACHE_H

#include <Vector>

class Light;

class LightCache {

public:

	LightCache();
	~LightCache();

	void AddLight(Light* pLight);
	std::vector<Light*> GetLights();
private:

	std::vector<Light*> _lights;	
};

#endif // !LIGHTCACHE_H

