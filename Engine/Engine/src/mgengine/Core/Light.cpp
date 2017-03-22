
#include <iostream>
using namespace std;

#include <glm.hpp>
#include "Light.h"
#include "Mesh.hpp"
#include "mge/config.hpp"
#include "mge/materials/ColorMaterial.hpp"


#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>

Light::Light(LightType pType, glm::vec3 pPosition, glm::vec3 pDirection, glm::vec3 pColor, glm::vec3 pAmbientColor, glm::vec3 pAttenuation, glm::vec2 pFallOff, std::string pName)
	: GameObject(pName, pPosition), _lightType(pType), _position(pPosition), _direction(pDirection), _color(pColor), _ambientColor(pAmbientColor), _attenuation(pAttenuation), _fallOff(pFallOff)
{
	//cameraMesh = Mesh::load(config::MGE_MODEL_PATH + "camera.obj"); ////Rectangle
	//colorMaterial = new ColorMaterial(pColor);
	//	scale(glm::vec3(.1f, .1f, .1f));
	//setMesh(cameraMesh);
	//setMaterial(colorMaterial);
	_lightType = pType;

	//ambientColor = glm::vec4(_ambientColor,1) ;
	LightColor = glm::vec4(pColor, 1);
	//ambientIntensity = .0f;
	directionalLightColor = glm::vec3(1, 1, 0);


}

Light::~Light()
{
}

Light * Light::getLight()
{
	return this;
}

void Light::setAttenuation(float pConstantAttenuation, float pLinearAttenuation, float pQuadraticAttenuation)
{
	constantAttenuation = pConstantAttenuation;
	linearAttenuation = pLinearAttenuation;
	quadraticAttenuation = pQuadraticAttenuation;
}

void Light::setSpotLight(float pSpotCutoff, float pSpotExponent, glm::vec3 pSpotDirection)
{
	spotCutoff = pSpotCutoff;
	spotExponent = pSpotExponent;
	spotDirection = pSpotDirection;
}

glm::vec3 Light::getAmbientColor()
{
	return _ambientColor;
}
/*
float Light::getAmbientIntensity()
{
return _ambientIntensity;
}
*/

void Light::SetAmbientLight(float pIntensity, glm::vec3 pColor)
{
	// Light:: _ambientIntensity = pIntensity;
	Light::_ambientColor = pColor;
}

int Light::GetLightType()
{
	switch (_lightType)
	{
	case DIRECTIONAL:
		return 0;
		break;
	case POINT:
		return 1;
		break;
	case SPOT:
		return 2;
		break;
	}

	return 0;
}
glm::vec3 Light::GetDirection()
{
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f;

/*	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		turnSpeed = -45;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		turnSpeed = +45;
	}*/

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, than we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	if (_lightType == LightType::DIRECTIONAL)
		_direction = glm::rotate(_direction, glm::radians(turnSpeed*.016f), glm::vec3(0.0f, 1.0f, 0.0f));
	return _direction;
}

glm::vec3 Light::GetPosition()
{
	_position = getWorldPosition();
	return _position;
}

glm::vec3 Light::GetColor()
{
	return _color;
}

glm::vec3 Light::GetAmbient()
{
	return _ambientColor;
}

glm::vec3 Light::GetAttenuation()
{
	return _attenuation;
}

glm::vec2 Light::GetFallOff() {
	return _fallOff;
}
