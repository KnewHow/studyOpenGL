#version 450 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

in vec2 texCoords;

out vec4 frag_Color;

void main() {
    frag_Color = texture(material.texture_diffuse1, texCoords);
}