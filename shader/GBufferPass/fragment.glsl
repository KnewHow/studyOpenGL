#version 450 core

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpce;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

void main() {
    gPos = fs_in.fragPos;
    gNormal = fs_in.normal;
    gAlbedoSpce.rgb = texture(texture_diffuse1, fs_in.texcoords).rgb;
    gAlbedoSpce.a = texture(texture_specular1, fs_in.texcoords).r;
}