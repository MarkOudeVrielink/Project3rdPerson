#ifndef LIGHTINFO_H
#define LIGHTINFO_H
#include "mge/core/ShaderProgram.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include <vector>


class LightInfo {
public:
	LightInfo(int pIndex, ShaderProgram* pShader);

	GLint _lightDirection;
	GLint _lightPosition;
	GLint _lightColor;
	GLint _globalAmbient;
	GLint _lightAttenuation;
	GLint _innerCone;
	GLint _outerCone;
	GLint _lightType;
};
#endif