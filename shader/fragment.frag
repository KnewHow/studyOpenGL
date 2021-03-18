#version 450 core

in vec4 vs_color;
in VS_OUT {
    vec4 color;
} fs_in;

out vec4 color;


void main(void) {
    color = fs_in.color;
}