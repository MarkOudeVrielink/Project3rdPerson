#ifndef LIGHTTYPE_H
#define LIGHTTYPE_H

#include "mge/Light/Light.h"

class DirectionalLight : public Light {
public:

	DirectionalLight(std::string pName = NULL, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 pDirection = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 pColor = glm::vec3(1.0f, 1.0f, 1.0f), float pIntensity = 1.0f);
	~DirectionalLight();
private:


};

class SpotLight : public Light {
public:
	SpotLight();
	~SpotLight();
};

class PointLight : public Light {
public:
	PointLight();
	~PointLight();
};
#endif // !LIGHTTYPE_H

