#version 450 core

out float frag_Color;

in vec2 texcoords;

uniform sampler2D gPos;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

uniform mat4 projection;

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

const vec2 noiseScale = vec2(1366.0 / 4.0, 768.0 / 4.0);


void main() {
    vec3 fragPos = texture(gPos, texcoords).rgb;
    vec3 normal = normalize(texture(gNormal, texcoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, texcoords * noiseScale).rgb);
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; i++) {
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz = offset.xyz / offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // to 0.1 ~ 1.0
        float sampleDepth = texture(gPos, offset.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    frag_Color = 1.0 - (occlusion / kernelSize);
}