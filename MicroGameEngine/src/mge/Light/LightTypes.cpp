#include "mge/Light/LightTypes.h"

/// DirectionalLight
DirectionalLight::DirectionalLight(std::string pName = NULL, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 pColor = glm::vec3(1.0f, 1.0f, 1.0f), float pIntensity = 1.0f) 
	: GameObject(pName, pPosition)
{

}

DirectionalLight::~DirectionalLight()
{

}

///SpotLight
SpotLight::SpotLight()
{

}

SpotLight::~SpotLight()
{

}

///PointLight
PointLight::PointLight()
{

}

PointLight::~PointLight()
{

}
