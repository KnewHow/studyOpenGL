#version 450 core

layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedo;

in VS_OUT {
    vec3 viewPos;
    vec3 normal;
    vec2 texcoords;
} fs_in;

void main() {
    gPos = fs_in.viewerPos;
    gNormal = normalize(fs_in.normal);
    gAlbedoSpce = vec4(0.95);
}