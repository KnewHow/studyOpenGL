#version 450 core

out vec4 color;

uniform vec4 dynamic_color;

void main(void) {
    color = dynamic_color;
}