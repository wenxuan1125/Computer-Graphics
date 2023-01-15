#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 M, V, P;
uniform float r;
uniform float g;
uniform float b;
uniform float time;
uniform float magnitude;
out vec4 color;

float rand(vec3 co){ return fract(sin(dot(co.xyz ,vec3(12.9898,78.233, 52.3256))) * 43758.5453); }

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{

    vec3 direction = normal * magnitude;
    return position + vec4(direction, 0.0f);
}

void main()
{

    vec3 normal = GetNormal();

    gl_Position = explode( P * V * M *gl_in[0].gl_Position, normal);
    color=vec4(1.0,1.0,1.0,1.0);
    EmitVertex();
    gl_Position = explode( P * V * M *gl_in[1].gl_Position, normal);
    color=vec4(1.0,1.0,1.0,1.0);
    EmitVertex();
    gl_Position = explode( P * V * M *gl_in[2].gl_Position, normal);
    color=vec4(1.0,1.0,1.0,1.0);
    EmitVertex();
    EndPrimitive();
    

    






    
}
