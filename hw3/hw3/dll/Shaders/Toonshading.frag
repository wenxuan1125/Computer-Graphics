#version 430

uniform sampler2D texture;
uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform vec3 Kd;


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

    float intensity;
    float level = dot( L, N);

   if( level > 0.95) intensity = 1;
   else if( level > 0.75) intensity = 0.8;
   else if( level > 0.50) intensity = 0.6;
   else if( level > 0.25) intensity = 0.4;
   else intensity = 0.2;

    vec3 object_color = texture2D(texture, uv).rgb;
    color = vec4(Kd * object_color * intensity, 1);

} 