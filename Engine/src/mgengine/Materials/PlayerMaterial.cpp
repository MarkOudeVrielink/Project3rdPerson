#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mgengine/Materials/PlayerMaterial.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* PlayerMaterial::_shader = NULL;

GLint PlayerMaterial::_uMVPMatrix = 0;
GLint PlayerMaterial::_uTime = 0;
GLint PlayerMaterial::_uInvulnerable = 0;

GLint PlayerMaterial::_aVertex = 0;
GLint PlayerMaterial::_aNormal = 0;
GLint PlayerMaterial::_aUV = 0;

PlayerMaterial::PlayerMaterial(Texture * pDiffuseTexture, float pBlinkRate) :_diffuseTexture(pDiffuseTexture), _blinkRate(pBlinkRate), _isInvulnerable(false)
{
	_lazyInitializeShader();
}

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

		_aVertex		= _shader->getAttribLocation("vertex");
		_aNormal		= _shader->getAttribLocation("normal");
		_aUV			= _shader->getAttribLocation("uv");

	}
}

void PlayerMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void PlayerMaterial::setInvulnerable(bool pValue)
{
	_isInvulnerable = pValue;
}

void PlayerMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse"), 0);

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	_time = (float) clock() / _blinkRate;
	
	glUniform1f(_uTime, _time);
	glUniform1i(_uInvulnerable, _isInvulnerable);

	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);	
}