#version 450 core

out vec4 frag_Color;

in vec3 fColor;

void main() {
    frag_Color = vec4(fColor, 1.0);
}