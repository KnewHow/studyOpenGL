#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform mat4 model_view_mat;
uniform mat4 projection_mat;

out vec4 vs_color;

void main(void) {
    gl_Position =  projection_mat * model_view_mat * position;
    vs_color = vec4(1.0, 0.0, 0.0, 1.0);
}