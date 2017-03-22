#include <glm.hpp>

#include "mge/materials/LightMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "LightInfo.h"
#include "Light.h"

#include "mge/config.hpp"

ShaderProgram* LightMaterial::_shader = NULL;


GLint LightMaterial::_aVertex = 0;
GLint LightMaterial::_aNormal = 0;
GLint LightMaterial::_aUV = 0;


GLint LightMaterial::_modelMatrixIndex = 0;
GLint LightMaterial::_viewMatrixIndex = 0;
GLint LightMaterial::_perspectiveMatrixIndex = 0;
GLint LightMaterial::_globalAmbientIndex = 0;
GLint LightMaterial::_diffuseMaterialColorIndex = 0;
GLint LightMaterial::_lightPositionIndex = 0;
GLint LightMaterial::_lightDirectionIndex = 0;
GLint LightMaterial::_lightColorIndex = 0;
GLint LightMaterial::_lightAttenuationIndex = 0;
GLint LightMaterial::_eyeMatrixIndex = 0;
GLint LightMaterial::_specularColorIndex = 0;
GLint LightMaterial::_specularShininessIndex = 0;
GLint LightMaterial::_innerConeIndex = 0;
GLint LightMaterial::_outerConeIndex = 0;
GLint LightMaterial::_lightTypeIndex = 0;

LightMaterial::LightMaterial(Texture * pDiffuseTexture):_diffuseTexture(pDiffuseTexture) {
    _lazyInitializeShader();
	for (int i = 0; i < 4; i++) {
		_lightInfo->push_back(LightInfo(i, _shader));
	}
	
}

LightMaterial::~LightMaterial() {}

void LightMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"light.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"light.fs");
        _shader->finalize();

		_modelMatrixIndex = _shader->getUniformLocation("modelMatrix");
		_viewMatrixIndex = _shader->getUniformLocation("viewMatrix");
		_perspectiveMatrixIndex = _shader->getUniformLocation("perspectiveMatrix");

		_eyeMatrixIndex = _shader->getUniformLocation("eyeMatrix");

		//_diffuseMaterialColorIndex = _shader->getUniformLocation("diffuseColor");
		_specularColorIndex = _shader->getUniformLocation("specularColor");
		_specularShininessIndex = _shader->getUniformLocation("shininess");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
    }
	//indices for uniforms

	
}

void LightMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void LightMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, std::list<Light*> *pLights = NULL)
{
    if (!_diffuseTexture) return;

    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 0);

    //pass in all MVP matrices separately
	glUniformMatrix4fv(_perspectiveMatrixIndex, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_viewMatrixIndex, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_modelMatrixIndex, 1, GL_FALSE, glm::value_ptr(pModelMatrix));



	glUniformMatrix4fv(_eyeMatrixIndex, 1, GL_FALSE, glm::value_ptr(glm::inverse(pViewMatrix)));
	Light * light;
	for (std::list<Light*>::iterator it = pLights->begin(); it != pLights->end(); ++it) {
		_lights.push_back((*it)->getLight());
	}

//	glUniform3fv(_diffuseMaterialColorIndex, 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_specularColorIndex, 1, glm::value_ptr(_specularColor));
	glUniform1f(_specularShininessIndex, _specularShininess);


	for (int i = 0; i < _lights.size() && i < 3; i++) {
		glUniform3fv(_lightInfo->at(i)._lightDirection, 1, glm::value_ptr(_lights.at(i)->GetDirection()));
		glUniform3fv(_lightInfo->at(i)._lightColor, 1, glm::value_ptr(_lights.at(i)->GetColor()));
		glUniform3fv(_lightInfo->at(i)._globalAmbient, 1, glm::value_ptr(_lights.at(i)->GetAmbient()));
		glUniform3fv(_lightInfo->at(i)._lightPosition, 1, glm::value_ptr(_lights.at(i)->GetPosition()));
		glUniform3fv(_lightInfo->at(i)._lightAttenuation, 1, glm::value_ptr(_lights.at(i)->GetAttenuation()));
		glUniform1f(_lightInfo->at(i)._innerCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().x)));
		glUniform1f(_lightInfo->at(i)._outerCone, glm::cos(glm::radians(_lights.at(i)->GetFallOff().y)));
		glUniform1i(_lightInfo->at(i)._lightType, _lights.at(i)->GetLightType());
	}


    //now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
