#version 450 core

layout(location = 0) out vec4 frag_Color;
layout(location = 1) out vec4 bright_Color;

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
    vec3 result = ambient + diffuse;
    
    // dot product with the threshold,then check whether result is enough heigher
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) {
        bright_Color = vec4(result, 1.0);
    } else {
        bright_Color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    frag_Color = vec4(result, 1.0);
}