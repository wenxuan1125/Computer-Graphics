#version 430

//// TODO: ////
//
// Hint:
// 1. Receive position, normal, texcoord from bind buffer
// 2. Receive Model matrix, View matrix, and Projection matrix from uniform
// 3. Pass texcoord and Normal to fragment shader (normal will not use in this homework)
// 4. Calculate view space by gl_Position (must be vec4)
//
layout(location=0) in vec3 position_to_vert;
layout(location=1) in vec3 normal_to_vert;
layout(location=2) in vec2 texcoord_to_vert;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 textcoord_to_frag;
out vec3 normal_to_frag;


void main()
{
	gl_Position = P * V * M * vec4(position_to_vert, 1.0);
	textcoord_to_frag = texcoord_to_vert;
	normal_to_frag = normal_to_vert;

  
}
