#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 inFragPosition;
out vec3 inNormal;
out vec2 texCoords;

void main() {
    texCoords = aTexCoords;
    inFragPosition = vec3(model * vec4(aPosition, 1.0));
    inNormal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}