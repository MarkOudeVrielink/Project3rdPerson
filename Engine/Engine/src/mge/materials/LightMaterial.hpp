#pragma once
#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "LightInfo.h"
#include "Light.h"
/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class LightMaterial : public AbstractMaterial
{
    public:
        LightMaterial (Texture* pDiffuseTexture);
        virtual ~LightMaterial ();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, std::list<Light*> *pLights) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _diffuseTexture;


		

        LightMaterial(const LightMaterial&);
        LightMaterial& operator=(const LightMaterial&);
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

		//this ones is unique per instance of color material
		glm::vec3 _diffuseColor;
		glm::vec3 _lightDirection;
		glm::vec3 _lightPosition;
		glm::vec3 _lightColor;
		glm::vec3 _lightAttenuation;
		glm::vec3 _specularColor = glm::vec3(1,1,1);
		float _specularShininess = 0;
		float _innerCone;
		float _outerCone;
		int _lightType;

		static GLint _aVertex;
		static GLint _aNormal;
		static GLint _aUV;

		Light* _light;
		std::vector<Light*> _lights;
		vector<LightInfo>*  _lightInfo = new vector<LightInfo>();
};

#endif // TEXTUREMATERIAL_H
