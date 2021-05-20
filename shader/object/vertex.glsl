#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isReverseNormal;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    if(isReverseNormal) {
        vs_out.normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
    } else {
        vs_out.normal = transpose(inverse(mat3(model))) * aNormal;
    }
    vs_out.texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}