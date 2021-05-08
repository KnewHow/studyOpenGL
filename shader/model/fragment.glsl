#version 450 core

uniform samplerCube cubemapTex;
uniform vec3 viewerPos;

in vec2 texCoords;
in vec3 inFragPosition;
in vec3 inNormal;

out vec4 color;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(inFragPosition - viewerPos);
    // vec3 R = reflect(I, normalize(inNormal));
    vec3 R = refract(I, normalize(inNormal), ratio);
    color = vec4(texture(cubemapTex, R).rgb, 1.0);
}