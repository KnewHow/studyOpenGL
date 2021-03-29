#version 450 core

layout(location = 0) in vec3 position;

uniform float horizontan_soffset;

void main(void) {
    gl_Position = vec4(position.x + horizontan_soffset, -position.y, position.z, 1.0f);
}