#version 330

#define MAX_LIGHTS = 4;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

in vec3 vertex;
in vec3 normal;

out vec3 worldNormal;
out vec3 worldVertex;

void main (void) {
    gl_Position = mvpMatrix * vec4(vertex,1);	

	worldNormal = vec3 (modelMatrix * vec4 (normal, 0));
	worldVertex = vec3 (modelMatrix * vec4 (vertex, 1));
	
	/*	
    //make sure normal is in same space as light direction. You should be able to explain the 0.
    vec3 worldNormal = vec3 (modelMatrix * vec4 (normal, 0));
    //take the dot of the direction from surface to light with the world surface normal
    diffuseIntensity = max (0, dot(-normalize(directionalLightVector), normalize (worldNormal)));
	*/	
}

