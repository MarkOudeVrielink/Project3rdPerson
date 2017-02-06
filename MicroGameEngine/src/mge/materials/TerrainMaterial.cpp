#include <glm.hpp>

#include "mge/materials/TerrainMaterial.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Light/Light.h"
#include "mge/Light/LightCache.h"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

GLint TerrainMaterial::_uMVPMatrix = 0;
GLint TerrainMaterial::_uModelMatrix = 0;
GLint TerrainMaterial::_uEyeMatrix = 0;

GLint TerrainMaterial::_uTime = 0;

GLint TerrainMaterial::_uDiffuseColor = 0;
GLint TerrainMaterial::_uSpecularColor = 0;
GLint TerrainMaterial::_uShininess = 0;

GLint TerrainMaterial::_aVertex = 0;
GLint TerrainMaterial::_aNormal = 0;
GLint TerrainMaterial::_aUV = 0;

TerrainMaterial::TerrainMaterial(Texture* pHeightMap, Texture* pSplatMapTexture, Texture* pDiffuseTexture0, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3,
									LightCache* pLightCache, glm::vec3 pDiffuseColor, glm::vec3 pSpecColor, float pShininess)
	:_heightMapTexture(pHeightMap), _splatMapTexture(pSplatMapTexture),
	_diffuseTexture0(pDiffuseTexture0), _diffuseTexture1(pDiffuseTexture1), _diffuseTexture2 (pDiffuseTexture2), _diffuseTexture3(pDiffuseTexture3),
	_lightCache(pLightCache), _diffuseColor(pDiffuseColor), _specularColor(pSpecColor), _shininess(pShininess)
{
	_lazyInitializeShader();

	for (int i = 0; i < 4; i++) {
		_lightInfo->push_back(LightInfo(i, _shader));
	}
	
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "terrain.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "terrain.fs");
		_shader->finalize();

		_uMVPMatrix		= _shader->getUniformLocation("mvpMatrix");
		_uModelMatrix	= _shader->getUniformLocation("modelMatrix");
		_uEyeMatrix		= _shader->getUniformLocation("eyeMatrix");

		_uTime			= _shader->getUniformLocation("time");

		_uDiffuseColor	= _shader->getUniformLocation("diffuseColor");
		_uSpecularColor = _shader->getUniformLocation("specularColor");
		_uShininess		= _shader->getUniformLocation("shininess");

		_aVertex	= _shader->getAttribLocation("vertex");
		_aNormal	= _shader->getAttribLocation("normal");
		_aUV		= _shader->getAttribLocation("uv");
	}
}

void TerrainMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	//_diffuseTexture = pDiffuseTexture;
}

void TerrainMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture0) return;

	_shader->use();

#pragma region Terrain

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _heightMapTexture->getId());
	glUniform1i(_shader->getUniformLocation("heightMap"), 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _splatMapTexture->getId());
	glUniform1i(_shader->getUniformLocation("splatMap"), 1);

	//Textures
	glActiveTexture(GL_TEXTURE2 + 0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture0->getId());	
	glUniform1i(_shader->getUniformLocation("textureDiffuse0"), 2);

	glActiveTexture(GL_TEXTURE2 + 1);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse1"), 3);

	glActiveTexture(GL_TEXTURE2 + 2);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse2"), 4);

	glActiveTexture(GL_TEXTURE2 + 3);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse3"), 5);

#pragma endregion

	time = clock();	

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix,		1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(_uModelMatrix,	1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_uEyeMatrix,		1, GL_FALSE, glm::value_ptr(glm::inverse(pViewMatrix)));

	glUniform1f(_uTime,	time);

#pragma region Light

	glUniform3fv(_uDiffuseColor,	1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_uSpecularColor,	1, glm::value_ptr(_specularColor));
	glUniform1f(_uShininess,		_shininess);

	_lights = _lightCache->GetLights();

	for (int i = 0; i < _lights.size() && i < 4; i++) {
		glUniform3fv(_lightInfo->at(i)._uLightDirection, 1, glm::value_ptr(_lights.at(i)->GetDirection()));
		glUniform3fv(_lightInfo->at(i)._uLightColor, 1, glm::value_ptr(_lights.at(i)->GetColor()));
		glUniform3fv(_lightInfo->at(i)._uGlobalAmbient, 1, glm::value_ptr(_lights.at(i)->GetAmbient()));
		glUniform3fv(_lightInfo->at(i)._uLightPosition, 1, glm::value_ptr(_lights.at(i)->GetPosition()));
		glUniform3fv(_lightInfo->at(i)._uLightAttenuation, 1, glm::value_ptr(_lights.at(i)->GetAttenuation()));
		glUniform1f(_lightInfo->at(i)._uInnerCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().x)));
		glUniform1f(_lightInfo->at(i)._uOuterCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().y)));
		glUniform1i(_lightInfo->at(i)._uLightType, _lights.at(i)->GetLightType());
	}

#pragma endregion
	
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

LightInfo::LightInfo(int pIndex, ShaderProgram* pShader) {
	_uLightPosition		= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightPosition");
	_uLightDirection	= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightDirection");
	_uLightColor		= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightColor");
	_uGlobalAmbient		= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].globalAmbient");
	_uLightAttenuation	= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightAttenuation");
	_uInnerCone			= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].innerCone");
	_uOuterCone			= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].outerCone");
	_uLightType			= pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightType");
}

