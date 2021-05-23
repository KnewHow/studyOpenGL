#version 450 core

out vec4 frag_Color;

uniform sampler2D texture_mask;
uniform float exposure;
uniform bool withHRD;

in vec2 texCoords;

void main() {
    vec3 hdrColor = texture(texture_mask, texCoords).rgb;
    const float gamma = 2.2;
    if(withHRD)
    {
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        result = pow(result, vec3(1.0 / gamma));
        frag_Color = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        frag_Color = vec4(result, 1.0);
    }
}