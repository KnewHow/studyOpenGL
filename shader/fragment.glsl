#version 450 core

out vec4 color;

in vec2 uvCoords;
in vec3 ourColor;

uniform sampler2D ourTexture;

void main(void) {
    color = texture(ourTexture, uvCoords);
}