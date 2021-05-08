#version 450 core

layout(location = 0) vec3 aPosition;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoords;

void main() {
    texCoords = aPosition;
    gl_Position = projection * view * vec4(aPosition, 1.0);    
}