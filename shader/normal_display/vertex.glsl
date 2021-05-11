#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
    gl_Position = view * model * vec4(aPosition, 1.0);
    mat3 normalMat = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(normalMat * aNormal);
}