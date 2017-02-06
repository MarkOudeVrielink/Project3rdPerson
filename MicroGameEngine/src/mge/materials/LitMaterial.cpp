#include <glm.hpp>
#include <vector>
#include <array>

#include "mge/materials/LitMaterial.h"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/Light/LightCache.h"
#include "mge/Light/Light.h"

ShaderProgram* LitMaterial::_shader = NULL;

GLint LitMaterial::_uMVPMatrix = 0;
GLint LitMaterial::_uModelMatrix = 0;
GLint LitMaterial::_uEyeMatrix = 0;

GLint LitMaterial::_uDiffuseColor = 0;
GLint LitMaterial::_uSpecularColor = 0;
GLint LitMaterial::_uShininess = 0;

GLint LitMaterial::_uLightDirection = 0;
GLint LitMaterial::_uLightPosition = 0;
GLint LitMaterial::_uLightColor = 0;
GLint LitMaterial::_uGlobalAmbient = 0;
GLint LitMaterial::_uLightAttenuation = 0;
GLint LitMaterial::_uInnerCone = 0;
GLint LitMaterial::_uOuterCone = 0;
GLint LitMaterial::_uLightType = 0;

GLint LitMaterial::_aVertex = 0;
GLint LitMaterial::_aNormal = 0;
GLint LitMaterial::_aUV = 0;

LitMaterial::LitMaterial(Light* pLight, glm::vec3 pDiffuseColor, glm::vec3 pSpecColor, float pShininess, LightCache* plightCache) 
	: _light(pLight), _diffuseColor(pDiffuseColor),_specularColor(pSpecColor), _shininess(pShininess), _lightCache(plightCache)
{	
	_lazyInitializeShader();
}

void LitMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "lit.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "lit.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix			= _shader->getUniformLocation("mvpMatrix");
		_uModelMatrix		= _shader->getUniformLocation("modelMatrix");	
		_uEyeMatrix			= _shader->getUniformLocation("eyeMatrix");
		
		_uDiffuseColor		= _shader->getUniformLocation("diffuseColor");
		_uSpecularColor		= _shader->getUniformLocation("specularColor");
		_uShininess			= _shader->getUniformLocation("shininess");

		_uLightDirection	= _shader->getUniformLocation("lightDirection");
		_uLightPosition		= _shader->getUniformLocation("lightPosition");
		_uLightColor		= _shader->getUniformLocation("lightColor");
		_uGlobalAmbient		= _shader->getUniformLocation("globalAmbient");
		_uLightAttenuation	= _shader->getUniformLocation("lightAttenuation");
		_uInnerCone			= _shader->getUniformLocation("innerCone");
		_uOuterCone			= _shader->getUniformLocation("outerCone");
		_uLightType			= _shader->getUniformLocation("lightType");

		_aVertex			= _shader->getAttribLocation("vertex");
		_aNormal			= _shader->getAttribLocation("normal");
		_aUV				= _shader->getAttribLocation("uv");		
	}
}

LitMaterial::~LitMaterial()
{
	//dtor	
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
	_diffuseColor = pDiffuseColor;
}

void LitMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();
	_lights = _lightCache->GetLights();	
				
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv	(_uMVPMatrix,			1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv	(_uModelMatrix,			1, GL_FALSE, glm::value_ptr(pModelMatrix));	
	glUniformMatrix4fv	(_uEyeMatrix,			1, GL_FALSE, glm::value_ptr(glm::inverse(pViewMatrix)));
			
	glUniform3fv(_uDiffuseColor,	1,	glm::value_ptr(_diffuseColor));
	glUniform3fv(_uSpecularColor,	1,	glm::value_ptr(_specularColor));
	glUniform1f(_uShininess,			_shininess);
	
 	glUniform3fv(_uLightDirection,	1,	glm::value_ptr(_light->GetDirection()));
	glUniform3fv(_uLightColor,		1,	glm::value_ptr(_light->GetColor()));
	glUniform3fv(_uGlobalAmbient,	1,	glm::value_ptr(_light->GetAmbient()));
	glUniform3fv(_uLightPosition,	1,	glm::value_ptr(_light->GetPosition()));
	glUniform3fv(_uLightAttenuation,1,	glm::value_ptr(_light->GetAttenuation()));
	glUniform1f(_uInnerCone,			glm::cos(glm::radians(_light->GetFallOff().x)));
	glUniform1f(_uOuterCone,			glm::cos(glm::radians(_light->GetFallOff().y)));
	glUniform1i (_uLightType,			_light->GetLightType());

	//now inform mesh of where to stream it's data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

