#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mgengine/Materials/AnimationMaterial.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* AnimationMaterial::_shader = NULL;

GLint AnimationMaterial::_uMVPMatrix = 0;
GLint AnimationMaterial::_uTime = 0;
GLint AnimationMaterial::_uInvulnerable = 0;
GLint AnimationMaterial::_uCharged = 0;

GLint AnimationMaterial::_aVertex = 0;
GLint AnimationMaterial::_aNormal = 0;
GLint AnimationMaterial::_aUV = 0;

AnimationMaterial::AnimationMaterial(Texture * pDiffuseTexture0, Texture * pDiffuseTexture1, float pBlinkRate) 
	:_diffuseTexture0(pDiffuseTexture0), _diffuseTexture1(pDiffuseTexture1),
	_blinkRate(pBlinkRate), _isInvulnerable(false), _isCharged(false)
{
	_lazyInitializeShader();
}

AnimationMaterial::~AnimationMaterial() {}

void AnimationMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "animation.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "animation.fs");
		_shader->finalize();

		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uTime = _shader->getUniformLocation("time");
		_uInvulnerable = _shader->getUniformLocation("invulnerable");
		_uCharged = _shader->getUniformLocation("charged");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void AnimationMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	//_diffuseTexture = pDiffuseTexture;
}

void AnimationMaterial::setInvulnerable(bool pValue)
{
	_isInvulnerable = pValue;
}

void AnimationMaterial::setCharged(bool pValue)
{
	_isCharged = pValue;
}

void AnimationMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture0) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture0->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse0"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse1"), 0);

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	_time = (float)clock() / _blinkRate;

	glUniform1f(_uTime, _time);
	glUniform1i(_uInvulnerable, _isInvulnerable);
	glUniform1i(_uCharged, _isCharged);

	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}