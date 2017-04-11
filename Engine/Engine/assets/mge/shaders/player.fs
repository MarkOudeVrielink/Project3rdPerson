//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D 	textureDiffuse;
uniform float 		time;
uniform bool 		invulnerable;
uniform bool		charged;

in vec2 			texCoord;


//LIGHTNING implementation
const int NUMBEROFLIGHTS = 4;

struct lightInfo{
    vec3 lightPosition;
    vec3 lightDirection;
    vec3 lightColor;
    vec3 globalAmbient;
    vec3 lightAttenuation;
    float innerCone;
    float outerCone;
    int lightType;
};
uniform mat4 eyeMatrix;//Inverse View
uniform vec4 specularColor;
uniform float shininess;
uniform lightInfo lights[4];
vec3 eyePosition = vec3(eyeMatrix[3]);
float diffuseIntensity;

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 fragment_color;
vec4 CalculateLights();
vec4 CalculateDirectional(lightInfo pLight);
vec4 CalculatePoint(lightInfo pLight);
vec4 CalculateSpot(lightInfo pLight);


vec4 pixelColor;

void main( void ) {

    pixelColor = CalculateLights()*  texture(textureDiffuse,texCoord);
	if(invulnerable){
		pixelColor.a *= (1 + sin(time));
	}
	fragment_color = pixelColor;
}

vec4 CalculateLights() {
    vec4 outColor;
	for(int i = 0; i < NUMBEROFLIGHTS; i++){
		if(lights[i].lightType == 0){
			outColor += CalculateDirectional(lights[i]);
		} else if(lights[i].lightType == 1){
			outColor += CalculatePoint(lights[i]);
		} else{
			outColor += CalculateSpot(lights[i]);
		}
	}
	return outColor;
}
vec4 CalculateDirectional(lightInfo pLight){
	diffuseIntensity = max (0, dot( normalize (worldNormal),normalize(-pLight.lightDirection)));

    vec3 ambientTerm =clamp( pLight.globalAmbient,0,1);
    vec3 diffuseTerm = clamp(diffuseIntensity * pLight.lightColor,0,1) ;
    vec3 specularTerm = vec3(0.0, 0.0, 0.0);

    vec3 vertexToEye = normalize(eyePosition - worldPosition);
    vec3 lightReflect = normalize(reflect(pLight.lightDirection,normalize(worldNormal)));
	float specularFactor = dot(vertexToEye, lightReflect);

	if (specularFactor > 0.0) {
	    specularFactor = pow(specularFactor,32.5f);//add Specular Power
		specularTerm = pLight.lightColor*shininess*specularFactor;
	}
   return vec4 (ambientTerm + diffuseTerm+specularTerm , 1);
}

vec4 CalculatePoint(lightInfo pLight){
	float c1 = pLight.lightAttenuation.x;
	float c2 = pLight.lightAttenuation.y;
	float c3 = pLight.lightAttenuation.z;

	diffuseIntensity = max (0, dot(-normalize(worldPosition - pLight.lightPosition), normalize (worldNormal)));

	float lightDistance = distance(worldPosition, pLight.lightPosition);
	float attenuation = 1 / (c1 + c2 * lightDistance + c3 * lightDistance*lightDistance);

    vec3 ambientTerm = pLight.globalAmbient ;
    vec3 diffuseTerm = diffuseIntensity * pLight.lightColor;
    vec3 specularTerm = vec3(0.0, 0.0, 0.0);

    vec3 vertexToEye = normalize(eyePosition - worldPosition);
    vec3 lightReflect = normalize(reflect(pLight.lightDirection,normalize(worldNormal)));
	float specularFactor = dot(vertexToEye, lightReflect);

	if (specularFactor > 0.0) {
	    specularFactor = pow(specularFactor,32.5f);//add Specular Power
		specularTerm = pLight.lightColor*shininess*specularFactor;
	}
	ambientTerm *= attenuation;
	diffuseTerm *= attenuation;
	specularTerm *= attenuation;

   return vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}

vec4 CalculateSpot(lightInfo pLight){

    float c1 = pLight.lightAttenuation.x;
	float c2 = pLight.lightAttenuation.y;
	float c3 = pLight.lightAttenuation.z;
    //attenuation based on distance light-object
	float lightDistance = distance(worldPosition, pLight.lightPosition);
	float attenuation = 1 / (c1 + c2 * lightDistance + c3 * lightDistance*lightDistance);

	vec3 direction = normalize(pLight.lightPosition - worldPosition);
	float angle = dot(direction, normalize(-pLight.lightDirection));

	float intensity = clamp((angle - pLight.outerCone)/(pLight.innerCone - pLight.outerCone), 0.0f, 1.0f);

    vec3 ambientTerm = pLight.globalAmbient ;
    vec3 diffuseTerm =  pLight.lightColor ;
    vec3 specularTerm = vec3(0.0, 0.0, 0.0);

    vec3 vertexToEye = normalize(eyePosition - worldPosition);
    vec3 lightReflect = normalize(reflect(pLight.lightDirection,normalize(worldNormal)));
	float specularFactor = dot(vertexToEye, lightReflect);

	if (specularFactor > 0.0) {
	    specularFactor = pow(specularFactor,32.5f);//add Specular Power
		specularTerm = pLight.lightColor*shininess*specularFactor;
	}
	diffuseTerm 	*= intensity*attenuation;
	specularTerm 	*= intensity*attenuation;

   return vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}

