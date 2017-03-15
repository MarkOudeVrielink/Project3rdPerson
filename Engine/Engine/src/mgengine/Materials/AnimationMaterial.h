#ifndef ANIMATIONMATERIAL_H
#define ANIMATIONMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class AnimationMaterial : public AbstractMaterial
{
public:
	AnimationMaterial(Texture* pDiffuseTexture0, Texture * pDiffuseTexture1,float pBlinkRate = 80.0f);
	virtual ~AnimationMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);
	void setInvulnerable(bool pValue);
	void setCharged(bool pValue);

private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();
	
	Texture* _diffuseTexture0;
	Texture* _diffuseTexture1;

	static GLint _uMVPMatrix;
	static GLint _uTime;
	static GLint _uInvulnerable;
	static GLint _uCharged;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	float _time = 0;
	float _blinkRate;

	bool _isInvulnerable;
	bool _isCharged;

	AnimationMaterial(const AnimationMaterial&);
	AnimationMaterial& operator=(const AnimationMaterial&);
};

#endif // ANIMATIONMATERIAL_H

