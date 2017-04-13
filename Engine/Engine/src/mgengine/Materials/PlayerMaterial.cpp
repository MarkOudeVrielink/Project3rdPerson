#include <glm.hpp>

#include "mgengine/Materials/PlayerMaterial.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* PlayerMaterial::_shader = NULL;

GLint PlayerMaterial::_uMVPMatrix		= 0;
GLint PlayerMaterial::_uTime			= 0;
GLint PlayerMaterial::_uInvulnerable	= 0;
GLint PlayerMaterial::_uCharged			= 0;

GLint PlayerMaterial::_aVertex = 0;
GLint PlayerMaterial::_aNormal = 0;
GLint PlayerMaterial::_aUV = 0;

GLint PlayerMaterial::_modelMatrixIndex = 0;
GLint PlayerMaterial::_viewMatrixIndex = 0;
GLint PlayerMaterial::_perspectiveMatrixIndex = 0;
GLint PlayerMaterial::_globalAmbientIndex = 0;
GLint PlayerMaterial::_diffuseMaterialColorIndex = 0;
GLint PlayerMaterial::_lightPositionIndex = 0;
GLint PlayerMaterial::_lightDirectionIndex = 0;
GLint PlayerMaterial::_lightColorIndex = 0;
GLint PlayerMaterial::_lightAttenuationIndex = 0;
GLint PlayerMaterial::_eyeMatrixIndex = 0;
GLint PlayerMaterial::_specularColorIndex = 0;
GLint PlayerMaterial::_specularShininessIndex = 0;
GLint PlayerMaterial::_innerConeIndex = 0;
GLint PlayerMaterial::_outerConeIndex = 0;
GLint PlayerMaterial::_lightTypeIndex = 0;

PlayerMaterial::PlayerMaterial(Texture * pDiffuseTexture, float pBlinkRate) :_diffuseTexture(pDiffuseTexture), _blinkRate(pBlinkRate), _isInvulnerable(false), _isCharged(false)
{
	_lazyInitializeShader();
	for (int i = 0; i < 4; i++) {
		_lightInfo->push_back(LightInfo(i, _shader));
	}
}
//hrllo
PlayerMaterial::~PlayerMaterial() {}

void PlayerMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "player.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "player.fs");
		_shader->finalize();

		_uMVPMatrix		= _shader->getUniformLocation("mvpMatrix");
		_uTime			= _shader->getUniformLocation("time");
		_uInvulnerable	= _shader->getUniformLocation("invulnerable");
		_uCharged		= _shader->getUniformLocation("charged");

		_modelMatrixIndex = _shader->getUniformLocation("modelMatrix");
		_viewMatrixIndex = _shader->getUniformLocation("viewMatrix");
		_perspectiveMatrixIndex = _shader->getUniformLocation("perspectiveMatrix");

		_eyeMatrixIndex = _shader->getUniformLocation("eyeMatrix");

		_specularColorIndex = _shader->getUniformLocation("specularColor");
		_specularShininessIndex = _shader->getUniformLocation("shininess");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

	}
}

void PlayerMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}
//how is life
void PlayerMaterial::setInvulnerable(bool pValue)
{
	_isInvulnerable = pValue;
}

void PlayerMaterial::setCharged(bool pValue)
{
	_isCharged = pValue;
}

void PlayerMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, std::list<Light*> *pLights) {
	if (!_diffuseTexture) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse"), 0);

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	//pass in all MVP matrices separately
	glUniformMatrix4fv(_perspectiveMatrixIndex, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_viewMatrixIndex, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	_time = (float) clock() / _blinkRate;
	
	glUniform1f(_uTime,				_time);
	glUniform1i(_uInvulnerable,		_isInvulnerable);
	glUniform1i(_uCharged,			_isCharged);


	glUniformMatrix4fv(_eyeMatrixIndex, 1, GL_FALSE, glm::value_ptr(glm::inverse(pViewMatrix)));
	Light * light;
	for (std::list<Light*>::iterator it = pLights->begin(); it != pLights->end(); ++it) {
		_lights.push_back((*it)->getLight());
	}

	//	glUniform3fv(_diffuseMaterialColorIndex, 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_specularColorIndex, 1, glm::value_ptr(_specularColor));
	glUniform1f(_specularShininessIndex, _specularShininess);


	for (int i = 0; i < _lights.size() && i < 4; i++) {
		glUniform3fv(_lightInfo->at(i)._lightDirection, 1, glm::value_ptr(_lights.at(i)->GetDirection()));
		glUniform3fv(_lightInfo->at(i)._lightColor, 1, glm::value_ptr(_lights.at(i)->GetColor()));
		glUniform3fv(_lightInfo->at(i)._globalAmbient, 1, glm::value_ptr(_lights.at(i)->GetAmbient()));
		glUniform3fv(_lightInfo->at(i)._lightPosition, 1, glm::value_ptr(_lights.at(i)->GetPosition()));
		glUniform3fv(_lightInfo->at(i)._lightAttenuation, 1, glm::value_ptr(_lights.at(i)->GetAttenuation()));
		glUniform1f(_lightInfo->at(i)._innerCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().x)));
		glUniform1f(_lightInfo->at(i)._outerCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().y)));
		glUniform1i(_lightInfo->at(i)._lightType, _lights.at(i)->GetLightType());
	}

	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);	
}