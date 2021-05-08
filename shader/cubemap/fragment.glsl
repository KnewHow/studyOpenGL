#version 450 core

out vec4 fragColor;

uniform samplerCube cubemap;

in vec3 texCoords;


void main() {
    fragColor = texture(cubemap, texCoords);
}