#version 450 core

out vec4 frag_Color;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main() {
    vec3 color = texture(screenTexture, texCoords).rgb;
    float grayColor = 0.21 * color.r + 0.71 * color.g + 0.08 * color.b;
    frag_Color = vec4(vec3(grayColor), 1.0);
}