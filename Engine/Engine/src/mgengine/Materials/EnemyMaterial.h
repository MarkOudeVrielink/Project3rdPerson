#ifndef ENEMYMATERIAL_H
#define ENEMYMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class EnemyMaterial : public AbstractMaterial
{
public:
	EnemyMaterial(Texture* pDiffuseTexture, float pBlinkRate = 80.0f);
	virtual ~EnemyMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix,std::list<Light*> *pLights) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);
	void setDamaged(bool pValue);

private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	Texture* _diffuseTexture;

	static GLint _uMVPMatrix;
	static GLint _uTime;
	static GLint _uDamaged;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	float _time = 0;
	float _blinkRate;

	bool _isDamaged;

	EnemyMaterial(const EnemyMaterial&);
	EnemyMaterial& operator=(const EnemyMaterial&);

};

#endif // ENEMYMATERIAL_H
