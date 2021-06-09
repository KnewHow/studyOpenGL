#version 450 core


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 worldPos;
    vec3 normal;
    vec3 texcoords;
} vs_out;

void main() {
    vs_out.texcoords = aTexcoords;
    vs_out.worldPos = vec3(model * vec4(aPos, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.normal = normalMatrix * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}