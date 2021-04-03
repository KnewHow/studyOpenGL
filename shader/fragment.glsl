#version 450 core

out vec4 color;

in vec2 uvCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAlpha;

void main(void) {
    color = mix(texture(texture1, uvCoords), texture(texture2, uvCoords), mixAlpha);
}