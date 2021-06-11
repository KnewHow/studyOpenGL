#version 450 core

out vec4 frag_Color;

in vec3 worldPos;

uniform samplerCube environment_map;

const float PI = 3.14159265359;

void main() {
    vec3 N = normalize(worldPos);
    vec3 irradiance = vec3(0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    float sampleDelta = 0.025;
    int sampleSum = 0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            vec3 tangentSample =vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            irradiance += texture(environment_map, sampleVec).rgb * cos(theta) * sin(theta);
            sampleSum++;
        }
    }
    irradiance *= (1.0 / float(sampleSum));
    frag_Color = vec4(irradiance, 1.0);
}