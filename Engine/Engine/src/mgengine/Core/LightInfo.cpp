#include "LightInfo.h"

LightInfo::LightInfo(int pIndex, ShaderProgram* pShader) {
	_lightPosition = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightPosition");
	_lightDirection = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightDirection");
	_lightColor = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightColor");
	_globalAmbient = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].globalAmbient");
	_lightAttenuation = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightAttenuation");
	_innerCone = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].innerCone");
	_outerCone = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].outerCone");
	_lightType = pShader->getUniformLocation("lights[" + to_string(pIndex) + "].lightType");
}