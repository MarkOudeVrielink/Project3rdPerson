#include "mge/Light/Light.h"


Light::Light(LightType pType, std::string pName, glm::vec3 pPosition, glm::vec3 pDirection,  glm::vec3 pColor, glm::vec3 pAmbientColor, glm::vec3 pAttenuation, glm::vec2 pFallOff)
	: GameObject(pName, pPosition),	_type(pType), _position(pPosition), _direction(pDirection), _color (pColor), _ambientColor(pAmbientColor),_attenuation (pAttenuation), _fallOff(pFallOff)
{
	
}

Light::~Light() {
	
}

int Light::GetLightType()
{
	switch (_type)
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
	//return glm::vec3(_transform[2]);	
	return _direction;
}

glm::vec3 Light::GetPosition()
{
	return getLocalPosition();
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
