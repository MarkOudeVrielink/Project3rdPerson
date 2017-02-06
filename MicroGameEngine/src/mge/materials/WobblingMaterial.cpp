#include "mge/materials/WobblingMaterial.h"

#include <glm.hpp>

#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"

ShaderProgram* WobblingMaterial::_shader = NULL;

GLint WobblingMaterial::_uMVPMatrix = 0;
GLint WobblingMaterial::_uTime = 0;

GLint WobblingMaterial::_aVertex = 0;
GLint WobblingMaterial::_aNormal = 0;
GLint WobblingMaterial::_aUV = 0;


WobblingMaterial::WobblingMaterial() 
{
	//every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
	_time = 0;
}

WobblingMaterial::~WobblingMaterial()
{
}

void WobblingMaterial::render(Mesh * pMesh, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix)
{
	_shader->use();

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	_time ++;	
	//float time = (float)sin(clock() * 2);
	glUniform1f(_uTime, _time/4);
	
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);

}

void WobblingMaterial::_lazyInitializeShader()
{
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "wobble.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "wobble.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uTime = _shader->getUniformLocation("time");
			
		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}
