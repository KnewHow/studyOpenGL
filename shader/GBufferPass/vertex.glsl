#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texcoords;
} vs_out;


void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.normal = normalize(transpose(inverse(mat3(model))) * aNormal);
    vs_out.texcoords = aTexcoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}