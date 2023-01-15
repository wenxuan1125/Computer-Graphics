#version 430

uniform sampler2D texture;
uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform float gloss;

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

    vec3 object_color = texture2D(texture, uv).rgb;
    vec3 ambient = La * Ka * object_color;
    vec3 diffuse = Ld * Kd * object_color * max( dot(L, N), 0);
    vec3 specular = Ls * Ks * pow( dot( V, R), gloss);

    color = vec4( ambient + diffuse + specular, 1);

} 