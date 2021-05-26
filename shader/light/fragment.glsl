#version 450 core

out vec4 frag_Color;

uniform vec3 lightColor;

void main() {
    frag_Color = vec4(lightColor, 1.0);
}