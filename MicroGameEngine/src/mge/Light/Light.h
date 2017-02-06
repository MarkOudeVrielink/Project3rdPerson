#ifndef LIGHT_H
#define LIGHT_H

#include "mge\core\GameObject.hpp"
enum LightType {
	DIRECTIONAL, POINT, SPOT
};

class Light :
	public GameObject
{
public:
	Light(LightType pType		=	LightType::DIRECTIONAL, 
		std::string pName		=	NULL, 
		glm::vec3 pPosition		=	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pDirection	=	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pColor		=	glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 pAmbientColor =	glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pAttenuation	=	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec2 pFalloff		=	glm::vec2(10, 45)
	);	
	
	virtual ~Light();

	int GetLightType();
	
	glm::vec3 GetDirection();
	glm::vec3 GetPosition();
	glm::vec3 GetColor();
	glm::vec3 GetAmbient();
	glm::vec3 GetAttenuation();
	glm::vec2 GetFallOff();
private:
	LightType _type;

	glm::vec3 _direction;
	glm::vec3 _position;
	glm::vec3 _color;
	glm::vec3 _ambientColor;
	glm::vec3 _attenuation;
	glm::vec2 _fallOff;
};

#endif // !LIGHT_H
