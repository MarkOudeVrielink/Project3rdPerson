//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D 	textureDiffuse;
uniform float 		time;
uniform bool 		damaged;

in vec2 			texCoord;

out vec4 			fragment_color;

vec4 pixelColor =  texture(textureDiffuse,texCoord);

void main( void ) {

	if(damaged){
		pixelColor.rgb *= (1 + sin(time));
	}	
	
	fragment_color = pixelColor;
}
