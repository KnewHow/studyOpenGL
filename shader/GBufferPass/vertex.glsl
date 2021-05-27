#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isInvertedNormals;

out VS_OUT {
    vec3 viewPos;
    vec3 normal;
    vec2 texcoords;
} vs_out;


void main() {
    vs_out.viewPos = vec3(view * model * vec4(aPos, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    vs_out.normal = normalize(normalMatrix * (isInvertedNormals ? -aNormal : aNormal));
    vs_out.texcoords = aTexcoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}