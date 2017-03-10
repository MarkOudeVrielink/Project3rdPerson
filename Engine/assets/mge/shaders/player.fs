//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D 	textureDiffuse;
uniform float 		time;
uniform bool 		invulnerable;
uniform bool		charged;

in vec2 			texCoord;

out vec4 			fragment_color;

vec4 pixelColor =  texture(textureDiffuse,texCoord);

void main( void ) {

	if(invulnerable){
		pixelColor.a *= (1 + sin(time));
	}	
	
	//if(charged){
		//pixelColor.rg *= (1+ sin(time/2));
		//clamp(pixelColor.rg, 0.3f, 1.0f);		
	//}
	
	fragment_color = pixelColor;
}
