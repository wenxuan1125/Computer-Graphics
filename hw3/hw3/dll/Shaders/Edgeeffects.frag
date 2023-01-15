#version 430

uniform sampler2D texture;
uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform vec3 object_color;


in vec2 uv;
in vec3 normal;
in vec3 worldPos;

out vec4 color;
void main()
{
    vec3 L = normalize(WorldLightPos - worldPos);
    vec3 V = normalize(WorldCamPos - worldPos);
    vec3 N = normal;
    vec3 R = 2 * dot( L, N) * N - L;

    color = vec4( object_color * pow(1- dot(N, V), 8), 1);

} 