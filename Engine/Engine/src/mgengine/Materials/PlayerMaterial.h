#ifndef PLAYERMATERIAL_H
#define PLAYERMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "LightInfo.h"
#include "Light.h"

class PlayerMaterial : public AbstractMaterial
{
public:
	PlayerMaterial(Texture* pDiffuseTexture, float pBlinkRate = 80.0f);
	virtual ~PlayerMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, std::list<Light*> *pLights) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);
	void setInvulnerable(bool pValue);
	void setCharged(bool pValue);

private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	Texture* _diffuseTexture;

	static GLint _uMVPMatrix;
	static GLint _uTime;
	static GLint _uInvulnerable;
	static GLint _uCharged;

	//MVP
	static GLint _modelMatrixIndex;
	static GLint _viewMatrixIndex;
	static GLint _perspectiveMatrixIndex;
	//Colors
	static GLint _globalAmbientIndex;
	static GLint _diffuseMaterialColorIndex;
	//Light Info
	static GLint _lightPositionIndex;
	static GLint _lightDirectionIndex;
	static GLint _lightColorIndex;
	static GLint _lightAttenuationIndex;
	//Specular
	static GLint _eyeMatrixIndex;
	static GLint _specularColorIndex;
	static GLint _specularShininessIndex;
	//Spot Light Cone
	static GLint _innerConeIndex;
	static GLint _outerConeIndex;

	static GLint _lightTypeIndex;
	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	glm::vec3 _specularColor = glm::vec3(1, 1, 1);
	float _specularShininess = 0;
	float _time = 0;
	float _blinkRate;

	bool _isInvulnerable;
	bool _isCharged;

	Light* _light;
	std::vector<Light*> _lights;
	vector<LightInfo>*  _lightInfo = new vector<LightInfo>();

	PlayerMaterial(const PlayerMaterial&);
	PlayerMaterial& operator=(const PlayerMaterial&);

};

#endif // PLAYERMATERIAL_H

