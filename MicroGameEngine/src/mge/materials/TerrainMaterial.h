#ifndef TERRAINMATERIAL_H
#define TERRAINMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include <vector>

class Light;
class LightCache;

class LightInfo {
public:
	LightInfo(int pIndex, ShaderProgram* pShader);

	GLint _uLightDirection;
	GLint _uLightPosition;
	GLint _uLightColor;
	GLint _uGlobalAmbient;
	GLint _uLightAttenuation;
	GLint _uInnerCone;
	GLint _uOuterCone;
	GLint _uLightType;
};

class TerrainMaterial : public AbstractMaterial
{
public:
	TerrainMaterial(Texture* pHeightMapTexture, Texture* pSplatMapTexture, Texture* pDiffuseTexture0, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3,
							LightCache* pLightCache, glm::vec3 pColor = glm::vec3(0.5f, 0, 0), glm::vec3 pSpecColor = glm::vec3(1, 0, 0), float pShininess = 1.0f);
	virtual ~TerrainMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);	

protected:
private:

	float time;
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();
	
	Texture* _heightMapTexture;
	Texture* _splatMapTexture;
	Texture* _diffuseTexture0;
	Texture* _diffuseTexture1;
	Texture* _diffuseTexture2;
	Texture* _diffuseTexture3;

	TerrainMaterial(const TerrainMaterial&);
	TerrainMaterial& operator=(const TerrainMaterial&);

	static GLint _uMVPMatrix;
	static GLint _uModelMatrix;
	static GLint _uEyeMatrix;

	static GLint _uTime;

	static GLint _uDiffuseColor;
	static GLint _uSpecularColor;
	static GLint _uShininess;

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
	
	glm::vec3	_diffuseColor;
	glm::vec3	_specularColor;
	float		_shininess;

	LightCache* _lightCache;
	vector<Light*>  _lights;
	vector<LightInfo>*  _lightInfo = new vector<LightInfo>();

};

#endif // TERRAINMATERIAL_H
