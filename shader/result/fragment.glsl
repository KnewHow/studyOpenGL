#version 450 core

out vec4 frag_Color;

uniform sampler2D texture_original;
uniform sampler2D texture_blurred;
uniform float exposure;
uniform bool withBloom;

in vec2 texCoords;

void main() {
    vec3 originalColor = texture(texture_original, texCoords).rgb;
    vec3 blurredColor = texture(texture_blurred, texCoords).rgb;
    const float gamma = 2.2;
    vec3 result = originalColor;
    if(withBloom)
    {
        result += blurredColor;
    }
    result = vec3(1.0) - exp(-result * exposure);
    result = pow(result, vec3(1.0 / gamma));
    frag_Color = vec4(result, 1.0);
}