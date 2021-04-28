#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uvCoords;
out vec3 ourColor;

void main(void) {
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    uvCoords = uv;
}