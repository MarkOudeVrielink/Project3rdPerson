//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

const float MAXLIGHTS = 4; 

uniform sampler2D splatMap;
uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;

uniform mat4 	eyeMatrix;

uniform vec3 	diffuseColor;
uniform vec3	specularColor;
uniform float 	shininess;

struct light{
	vec3 	lightPosition;
	vec3 	lightDirection;
	vec3 	lightColor;
	vec3 	globalAmbient;
	vec3 	lightAttenuation;
	float 	innerCone;
	float 	outerCone;
	int 	lightType;
};

uniform light lights[4];

uniform float time;

vec3 eyePosition = vec3(eyeMatrix[3]);

float diffuseIntensity;

vec3 View;
vec3 Reflection;

in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldVertex;

out vec4 fragment_color;

vec4 BlendTextures();
vec4 CalculateLight();

vec4 CalculateDirectional(light pLight);
vec4 CalculatePoint(light pLight);
vec4 CalculateSpot(light pLight);


void main( void ) {
	
	fragment_color = BlendTextures() * CalculateLight() * 3 ;
}

vec4 BlendTextures(){	
	vec4 splat = texture(splatMap, texCoord);
	vec4 tex0 = texture(textureDiffuse0, texCoord);
	vec4 tex1 = texture(textureDiffuse1, texCoord);
	vec4 tex2 = texture(textureDiffuse2, texCoord);
	vec4 tex3 = texture(textureDiffuse3, texCoord);
	
	if(splat.b > 0.5){	
		vec2 tc = texCoord;
		vec2 pos = -1.0 + 2.0 * tc;
		float len = length(pos);
		vec2 newUv = tc + (pos/len) * tan(len * 5.0 - time * 0.001) * 0.001;
		tex2 = texture(textureDiffuse2, newUv);	
	}
	
	return tex0 * splat.r + tex1 * splat.g + tex2 * splat.b + tex3 * splat.a;
		
}

vec4 CalculateLight(){
	vec4 outColor;	
	
	for(int i = 0; i < MAXLIGHTS; i++){
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

vec4 CalculateDirectional(light pLight){
	diffuseIntensity = max (0, dot(normalize(-pLight.lightDirection), normalize (worldNormal)));
	Reflection = normalize(reflect(normalize(pLight.lightDirection), normalize(worldNormal)));
		
	View = normalize(View + pLight.lightDirection);
				
	float specularFactor = pow(max(dot(Reflection, View), 0), shininess);
	
    vec3 ambientTerm = pLight.globalAmbient * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * pLight.lightColor * diffuseColor;
    vec3 specularTerm = specularFactor * pLight.lightColor * specularColor;

   return vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}

vec4 CalculatePoint(light pLight){
	float c1 = pLight.lightAttenuation.x;
	float c2 = pLight.lightAttenuation.y;
	float c3 = pLight.lightAttenuation.z;
	
	diffuseIntensity = max (0, dot(-normalize(worldVertex - pLight.lightPosition), normalize (worldNormal)));
	Reflection = normalize(reflect(normalize(pLight.lightPosition - worldVertex), normalize(worldNormal)));	
		
	float lightDistance = distance(worldVertex, pLight.lightPosition);		
	float attenuation = 1 / (c1 + c2 * lightDistance + c3 * sqrt(lightDistance));
		
	View = normalize(View + (worldVertex - pLight.lightPosition));
		
	float specularFactor = pow(max(dot(Reflection, View), 0), shininess);
	
    vec3 ambientTerm = pLight.globalAmbient * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * pLight.lightColor * diffuseColor;
    vec3 specularTerm = specularFactor * pLight.lightColor * specularColor;
	
	ambientTerm *= attenuation;
	diffuseTerm *= attenuation;
	specularTerm *= attenuation;
	
   return vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}

vec4 CalculateSpot(light pLight){	
	vec3 vDir = normalize(pLight.lightPosition - worldVertex);
	float angle = dot(vDir, normalize(-pLight.lightDirection));
		
	float intensity = clamp((angle - pLight.outerCone)/(pLight.innerCone - pLight.outerCone), 0.0f, 1.0f);	
	Reflection = normalize(reflect(normalize(pLight.lightPosition - worldVertex), normalize(worldNormal)));
			
	View = normalize(View + pLight.lightDirection);

	float specularFactor = pow(max(dot(Reflection, View), 0), shininess);
	
    vec3 ambientTerm = pLight.globalAmbient * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * pLight.lightColor * diffuseColor;
    vec3 specularTerm = specularFactor * pLight.lightColor * specularColor;
	
	diffuseTerm 	*= intensity;
	specularTerm 	*= intensity;

   return vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}