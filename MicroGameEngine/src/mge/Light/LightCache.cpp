#include "mge/Light/LightCache.h"

#include <iostream>
#include <iterator>
#include <algorithm>

#include "mge/Light/Light.h"

LightCache::LightCache() {

}

LightCache::~LightCache() {

}

void LightCache::AddLight(Light * pLight)
{
	_lights.push_back(pLight);
}

std::vector<Light*> LightCache::GetLights()
{	
	return _lights;
}


