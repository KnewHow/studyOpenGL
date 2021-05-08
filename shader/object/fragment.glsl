#version 450 core


out vec4 color;

uniform sampler2D texture1;

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoords;

void main(void) {
    color = vec4(texture(texture1, texCoords).rgb, 1.0);
}