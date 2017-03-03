#ifndef PLAYERMATERIAL_H
#define PLAYERMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class PlayerMaterial : public AbstractMaterial
{
public:
	PlayerMaterial(Texture* pDiffuseTexture, float pBlinkRate = 80.0f);
	virtual ~PlayerMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

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

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	float _time = 0;
	float _blinkRate;

	bool _isInvulnerable;
	bool _isCharged;

	PlayerMaterial(const PlayerMaterial&);
	PlayerMaterial& operator=(const PlayerMaterial&);

};

#endif // PLAYERMATERIAL_H

