#version 450 core

uniform sampler2D texture1;

in vec2 texCoords;

out vec4 color;

void main(void) {
   color = texture(texture1, texCoords);
}