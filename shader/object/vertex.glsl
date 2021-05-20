#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewerPos;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec3 tangentSpaceViewerPos;
    vec3 tangentSpaceLightPos;
    vec3 tangentSpaceFragPos;
} vs_out;



void main(void) {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.texCoords = aTexCoords;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}