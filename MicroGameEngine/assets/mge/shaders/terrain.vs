//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1


uniform	mat4 	mvpMatrix;
uniform	mat4 	modelMatrix;

uniform sampler2D textureDiffuse;
uniform sampler2D heightMap;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

float maxHeight = 2;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 worldNormal;
out vec3 worldVertex;

vec3 RecalculateNormal();

void main( void ){
	texCoord = uv;

	vec4 heightColor = texture(heightMap, uv);	
	vec4 heightOffset = vec4(vertex.x, vertex.y + heightColor.y * maxHeight, vertex.z, 1);
		
	worldNormal = vec3(modelMatrix * vec4(RecalculateNormal(), 0));
	worldVertex = vec3(modelMatrix * heightOffset);
	
	gl_Position = mvpMatrix * heightOffset;
	
}

vec3 RecalculateNormal(){
	float distanceToNext = 0.1;

	vec4 colorLeft = texture(heightMap, vec2(uv.x - distanceToNext, uv.y));
	vec4 offsetLeft = vec4(vertex.x - distanceToNext, vertex.y + colorLeft.y * maxHeight, vertex.z, 1);
	
	vec4 colorRight = texture(heightMap, vec2(uv.x + distanceToNext, uv.y));
	vec4 offsetRight = vec4(vertex.x + distanceToNext, vertex.y + colorRight.y * maxHeight, vertex.z, 1);
	
	vec4 colorUp = texture(heightMap, vec2(uv.x, uv.y - distanceToNext));
	vec4 offsetUp = vec4(vertex.x, vertex.y - colorUp.y * maxHeight, vertex.z - distanceToNext, 1);
	
	vec4 colorDown = texture(heightMap, vec2(uv.x, uv.y + distanceToNext));
	vec4 offsetDown = vec4(vertex.x, vertex.y - colorDown.y * maxHeight, vertex.z + distanceToNext, 1);
	
	vec3 horizontal = vec3(normalize(offsetRight - offsetLeft));
	vec3 vertical = vec3(normalize(offsetDown - offsetUp));
	
	return cross(horizontal, vertical);
}