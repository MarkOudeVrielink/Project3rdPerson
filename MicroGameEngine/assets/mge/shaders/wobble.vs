//WOBBLE VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 mvpMatrix;
uniform float time;

vec3 newVertex;

float amplitude = 0.08f;

void main( void ){
	newVertex = vertex;
	newVertex.y -= sin(vertex.x + time) * amplitude;	
	newVertex.x -= sin(vertex.y + time) * amplitude;
	//newVertex.z += sin(vertex.x + time) * amplitude;	
	gl_Position = mvpMatrix * vec4(newVertex, 1.f);
}
