#version 450 core

out vec4 frag_Color;

struct Light {
    vec3 pos;
    vec3 color;

    float linear;
    float quadratic;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewerPos;

uniform sampler2D texture_pos;
uniform sampler2D texture_normal;
uniform sampler2D texture_albedo_spec;


in vec2 texcoords;

void main() {
    vec3 fragPos = texture(texture_pos, texcoords).rgb;
    vec3 normal = normalize(texture(texture_normal, texcoords).rgb);
    vec4 albedoSpec = texture(texture_albedo_spec, texcoords);
    vec3 color = albedoSpec.rgb;
    float specularIntension = albedoSpec.a;

    vec3 result = 0.1 * color;
    for(int i = 0; i < NR_LIGHTS; i++) {

        vec3 lightDir = normalize(lights[i].pos - fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color * lights[i].color;

        vec3 viewerDir = normalize(viewerPos - fragPos);
        vec3 halfVec = normalize(lightDir + viewerDir);
        float spec = pow(max(dot(halfVec, normal), 0.0), 32.0);
        vec3 specular = spec * specularIntension * lights[i].color;

        float distance = length(fragPos - lights[i].pos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        result += (diffuse + specular) * attenuation;
    }
    frag_Color = vec4(result, 1.0);

}