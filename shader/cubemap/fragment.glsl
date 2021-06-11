#version 450 core

out vec4 frag_Color;

in vec3 worldPos;

uniform samplerCube environment_map;

void main() {
    vec3 color = texture(environment_map, worldPos).rgb;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    frag_Color = vec4(color, 1.0);
}