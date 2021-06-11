#version 450 core

layout(location = 0) in vec3 aPos;
out vec3 worldPos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    worldPos = aPos;
    gl_Position = view * projection * vec4(aPos, 1.0);
}