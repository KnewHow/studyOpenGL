#version 450 core

out vec4 frag_Color;

uniform sampler2D texture_mask;
uniform float z_near;
uniform float z_far;

in vec2 texCoords;

float getLinearDepth(float depth) { // transform depth to linear depth
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * z_near * z_far) / (z_far + z_near - ndc * (z_far - z_near));
}

void main() {
    float depthValue = texture(texture_mask, texCoords).r;
    //frag_Color = vec4(vec3(getLinearDepth(depthValue) / z_far), 1.0);
    frag_Color = vec4(vec3(depthValue), 1.0);
}