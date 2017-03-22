#pragma once

#include <string>
#include "GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
enum LightType
{
	DIRECTIONAL, POINT, SPOT
};
class Light :
	public GameObject
{
public:

	Light(
		LightType pType = LightType::DIRECTIONAL,
		glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pDirection = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pColor = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 pAmbientColor = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 pAttenuation = glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec2 pFalloff = glm::vec2(10, 45),
		std::string pName = "Light"
	);

	glm::vec3 ambientColor;// = glm::vec3(0, 1, 1);//static todo


	glm::vec3 LightColor = glm::vec3(1, 1, 1);

	glm::vec4 position = glm::vec4(0, 1, 0, 1);
	glm::vec3 directionalLightColor = glm::vec3(1, 1, 0);
	~Light();
	Mesh* cameraMesh;
	AbstractMaterial* colorMaterial;
	GameObject* lightGizmo;
	Light *getLight();

	int GetLightType();
	glm::vec3 GetDirection();
	glm::vec3 GetPosition();
	glm::vec3 GetColor();
	glm::vec3 GetAmbient();
	glm::vec3 GetAttenuation();
	glm::vec2 GetFallOff();

	void SetAmbientLight(float pIntensity, glm::vec3 pColor);
	void setAttenuation(float pConstantAttenuation, float pLinearAttenuation, float pQuadraticAttenuation);
	void setAttenuation(glm::vec3 pAttenatuation);
	void setSpotLight(float pSpotCutoff, float pSpotExponent, glm::vec3 pSpotDirection);

	glm::vec3 getAmbientColor();
	float getAmbientIntensity();

	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.2f;
	float quadraticAttenuation = 0.0f;
	float spotCutoff = 180;
	float spotExponent = 10;
	glm::vec3 spotDirection = glm::vec3(-1.0f, -0.5f, 1);
	glm::vec4 specular = glm::vec4(1.0, 1.0, 1.0, 1.0);

private:
	//Add ambient intensity
	LightType _lightType;

	glm::vec3 _direction;
	glm::vec3 _position;
	glm::vec3 _color;
	glm::vec3 _ambientColor;
	glm::vec3 _attenuation;
	glm::vec2 _fallOff;

	float turnSpeed;


};


