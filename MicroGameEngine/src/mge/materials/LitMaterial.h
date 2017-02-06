#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#define MAX_LIGHTS = 4;

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"

#include <Vector>

class LightCache;
class Light;

class LitMaterial : public AbstractMaterial
{
public:
	LitMaterial(
		Light* pLight,
		glm::vec3 pColor		= glm::vec3(0.5f, 0, 0), 
		glm::vec3 pSpecColor	= glm::vec3(1, 0, 0),
		float pShininess		= 1.0f,
		LightCache* pLightCache = NULL
	);

	virtual ~LitMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);

private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uModelMatrix;	
	static GLint _uEyeMatrix;

	static GLint _uDiffuseColor;
	static GLint _uSpecularColor;
	static GLint _uShininess;

	//static GLint _uLightDirection[];
	//static GLint _uLightPosition[];
	//static GLint _uLightColor[];
	//static GLint _uLightType[];

	static GLint _uLightDirection;
	static GLint _uLightPosition;
	static GLint _uLightColor;
	static GLint _uGlobalAmbient;
	static GLint _uLightAttenuation;
	static GLint _uInnerCone;
	static GLint _uOuterCone;
	static GLint _uLightType;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;	

	//this one is unique per instance of color material
	glm::vec3	_diffuseColor;
	glm::vec3	_specularColor;
	float		_shininess;

	//glm::vec3	_lightDirection[MAX_LIGHTS];
	//glm::vec3	_lightPosition[MAX_LIGHTS];
	//glm::vec3	_lightColor[MAX_LIGHTS];
	//int		_lightType[MAX_LIGHTS];
	
	glm::vec3	_lightDirection;
	glm::vec3	_lightPosition;
	glm::vec3	_lightColor;
	glm::vec3	_lightAttenuation;
	float		_innerCone;
	float		_outerCone;
	int			_lightType;

	LightCache* _lightCache;
	Light*		_light;

	std::vector<Light*>  _lights;		
};

#endif // LITMATERIAL_H
