#version 450 core

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedo;

uniform sampler2D texture_diffuse1;

in VS_OUT {
    vec3 viewPos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

void main() {
    gPos = fs_in.viewPos;
    gNormal = normalize(fs_in.normal);
    gAlbedo = texture(texture_diffuse1, fs_in.texcoords);
}