#version 450 core

out vec4 frag_Color;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light lights[4];
uniform sampler2D diffuse_texture;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;


void main() {
    vec3 color = texture(diffuse_texture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);

    vec3 ambient = 0.0 *  color;
    vec3 diffuse = vec3(0.0);
    for(int i = 0; i < 4; i++) {
        vec3 lightDir = normalize(lights[i].pos - fs_in.fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffColor = diff * lights[i].color * color;
        float distance = length(lights[i].pos - fs_in.fragPos);
        diffColor /= (distance * distance);
        diffuse += diffColor;
    }
    frag_Color = vec4(ambient + diffuse, 1.0);
}