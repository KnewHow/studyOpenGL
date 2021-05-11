#version 450 core

out vec4 frag_Color;

in vec2 outTexCoords;

uniform sampler2D texture_diffuse;

void main() {
    frag_Color = texture(texture_diffuse, outTexCoords);
}