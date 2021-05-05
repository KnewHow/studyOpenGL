#version 450 core

struct Material {
    sampler2D texture_diffuse1;
};


uniform Material material;

in vec2 texCoords;
in vec3 inFragPosition;
in vec3 inNormal;

out vec4 color;

void main() {
    color = vec4(texture(material.texture_diffuse1, texCoords).rgb, 1.0);
}