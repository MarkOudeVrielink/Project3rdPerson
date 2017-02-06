//WOBBLE FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

//uniform vec3 diffuseColor;
vec4 colour;
out vec4 fragment_color;

void main( void ) {
	
	if(gl_FragCoord.x > 0.9)
		colour = vec4 (1.0,0.0,0.0,1.0);
	else if(gl_FragCoord.x > 0.6)
		colour = vec4(1.0,1.0,0.0,1.0);
	else if (gl_FragCoord.x > 0.4)
		colour = vec4( 0.60,0.50,0.40,1.0);
	else
		colour = vec4 (1.0,1.0,1.0,1.0);
		
	if(gl_FragCoord.z >0.9)
		colour = vec4 (1.0,0.0,0.0,1.0);
	else if(gl_FragCoord.z > 0.6)
		colour = vec4(1.0,1.0,0.0,1.0);
	else if (gl_FragCoord.z > 0.4)
		colour = vec4 (0.60,0.50,0.40,1.0);
	else
		colour = vec4 (1.0,1.0,1.0,1.0);
	
	fragment_color = colour;
//	fragment_color = vec4 (0.0f,0.0f,1.0f,1);
}
