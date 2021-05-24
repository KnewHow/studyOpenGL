#version 450 core

layout(location = 0) out vec4 frag_Color;
layout(location = 1) out vec4 bright_Color;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform vec3 lightColor;

void main() {
    frag_Color = vec4(lightColor, 1.0);
    float brightness = dot(lightColor, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        bright_Color = vec4(lightColor, 1.0);
    } else {
        bright_Color = vec4(0.0, 0.0, 0.0, 1.0);
    }
}