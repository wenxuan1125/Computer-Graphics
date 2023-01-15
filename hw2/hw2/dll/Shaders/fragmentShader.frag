#version 430
//// TODO ////
//
// Hint:
// 1. Recieve texcoord and Normal from vertex shader
// 2. Calculate and return final color to opengl
//
in vec2 textcoord_to_frag;
in vec3 normal_to_frag;

uniform sampler2D Texture;
uniform sampler2D Texture2;
uniform int text2;
uniform float n;

out vec4 color_out;

void main()
{
	
	
	//color_out = texture2D(Texture2, textcoord_to_frag)+ vec4( 0.3, 0.5, 0.2, 1.0f) ;
		
	color_out = mix(texture2D(Texture, textcoord_to_frag), texture2D(Texture2, textcoord_to_frag), 0.3 * text2);
	color_out = color_out + vec4( n, n, n, 1.0f) ;
} 
