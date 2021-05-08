#version 450 core

uniform samplerCube cubemapTex;
uniform vec3 viewerPos;

in vec2 texCoords;
in vec3 inFragPosition;
in vec3 inNormal;

out vec4 color;

void main() {
    vec3 I = normalize(inFragPosition - viewerPos);
    vec3 R = reflect(I, normalize(inNormal));
    color = vec4(texture(cubemapTex, R).rgb, 1.0);
}