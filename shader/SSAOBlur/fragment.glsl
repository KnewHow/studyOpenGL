#version 450 core

out float frag_Color;

in vec2 texcoords;

uniform sampler2D texture_SSAO;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(texture_SSAO, 0));
    float result = 0.0;
    for(int x = -2; x < 2; x++) {
        for(int y = -2; y < 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(texture_SSAO, texcoords + offset).r;
        }
    }
    result = result / (4.0 * 4.0);
    frag_Color = result;
}