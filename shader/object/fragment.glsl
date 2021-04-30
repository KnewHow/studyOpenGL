#version 450 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(void) {
    color = vec4(objectColor * lightColor, 1.0);
}