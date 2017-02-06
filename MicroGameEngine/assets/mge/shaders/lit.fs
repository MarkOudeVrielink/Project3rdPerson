#version 330

#define MAX_LIGHTS = 4;

uniform mat4 	eyeMatrix;

uniform vec3 	diffuseColor;
uniform vec3	specularColor;
uniform float 	shininess;

uniform vec3 	lightPosition;
uniform vec3 	lightDirection;
uniform vec3 	lightColor;
uniform vec3 	globalAmbient;
uniform vec3 	lightAttenuation;
uniform float 	innerCone;
uniform float 	outerCone;
uniform int 	lightType;

vec3 eyePosition = vec3(eyeMatrix[3]);

float diffuseIntensity;
float c1 = lightAttenuation.x;
float c2 = lightAttenuation.y;
float c3 = lightAttenuation.z;

vec3 View;
vec3 Reflection;

in vec3 worldNormal;
in vec3 worldVertex;

out vec4 sColor;

void main (void) {	
	
	View = normalize(worldVertex - eyePosition);
	
	if(lightType == 0){
		diffuseIntensity = max (0, dot(-normalize(lightDirection), normalize (worldNormal)));
		Reflection = normalize(reflect(-normalize(lightDirection), normalize(worldNormal)));	
	} else if(lightType == 1){
		diffuseIntensity = max (0, dot(-normalize(worldVertex - lightPosition), normalize (worldNormal)));
		Reflection = normalize(reflect(normalize(lightPosition - worldVertex), normalize(worldNormal)));	
		
		float lightDistance = distance(worldVertex, lightPosition);		
		diffuseIntensity /= (c1 + c2 * lightDistance + c3 * sqrt(lightDistance));
	} else{
		vec3 vDir = normalize(worldVertex - lightPosition);
		float angle = dot(-vDir, normalize(-lightDirection));
		
		diffuseIntensity = clamp((angle - outerCone)/innerCone, 0.0f, 1.0f);	
		Reflection = normalize(reflect(normalize(lightPosition - worldVertex), normalize(worldNormal)));	
	}
	
	float specularFactor = pow(max(dot(Reflection, View), 0), shininess);
	
    vec3 ambientTerm = globalAmbient * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * lightColor * diffuseColor;
    vec3 specularTerm = specularFactor * lightColor * specularColor;

    sColor = vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}