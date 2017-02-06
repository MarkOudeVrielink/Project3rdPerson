#ifndef WOBBLINGMATERIAL_H
#define WOBBLINGMATERIAL_H

#include "E:\Docs\DesktopFolders\School\2nd Year\3D rendering\02\MicroGameEngine\src\mge\materials\AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"

class WobblingMaterial :
	public AbstractMaterial
{
public:
	WobblingMaterial();
	virtual ~WobblingMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

private:
	//all the static properties are shared between instances of ColorMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uTime;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	//this one is unique per instance of color material
	float _time;
};

#endif // WOBBLINGMATERIAL_H
