#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mgengine/Materials/EnemyMaterial.h"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* EnemyMaterial::_shader = NULL;

GLint EnemyMaterial::_uMVPMatrix = 0;
GLint EnemyMaterial::_uTime = 0;
GLint EnemyMaterial::_uDamaged = 0;

GLint EnemyMaterial::_aVertex = 0;
GLint EnemyMaterial::_aNormal = 0;
GLint EnemyMaterial::_aUV = 0;

EnemyMaterial::EnemyMaterial(Texture * pDiffuseTexture, float pBlinkRate) :_diffuseTexture(pDiffuseTexture), _blinkRate(pBlinkRate), _isDamaged(false)
{
	_lazyInitializeShader();
}

EnemyMaterial::~EnemyMaterial() {}

void EnemyMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "enemy.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "enemy.fs");
		_shader->finalize();

		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uTime = _shader->getUniformLocation("time");
		_uDamaged = _shader->getUniformLocation("damaged");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

	}
}

void EnemyMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void EnemyMaterial::setDamaged(bool pValue)
{
	_isDamaged = pValue;
}

void EnemyMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	glUniform1i(_shader->getUniformLocation("textureDiffuse"), 0);

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	_time = (float)clock() / _blinkRate;

	glUniform1f(_uTime, _time);
	glUniform1i(_uDamaged, _isDamaged);

	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}