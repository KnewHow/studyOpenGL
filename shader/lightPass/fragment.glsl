#version 450 core

out vec4 frag_Color;

struct Light {
    vec3 pos;
    vec3 color;

    float linear;
    float quadratic;
};

uniform Light light;
uniform vec3 viewerPos;

uniform sampler2D texture_pos;
uniform sampler2D texture_normal;
uniform sampler2D texture_albedo_spec;
uniform sampler2D texture_SSAO;


in vec2 texcoords;

void main() {
    vec3 fragPos = texture(texture_pos, texcoords).rgb;
    vec3 normal = normalize(texture(texture_normal, texcoords).rgb);
    vec3 color = texture(texture_albedo_spec, texcoords).rgb;
    float ambientOcclusion = texture(texture_SSAO, texcoords).r;

    vec3 ambient = vec3(0.3 * color * ambientOcclusion);

    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color * light.color;

    vec3 viewerDir = normalize(viewerPos - fragPos);
    vec3 halfVec = normalize(lightDir + viewerDir);
    float spec = pow(max(dot(halfVec, normal), 0.0), 32.0);
    vec3 specular = spec * light.color;

    float distance = length(fragPos - light.pos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
    vec3 result = (diffuse + specular) * attenuation;
    result += ambient;
    frag_Color = vec4(result, 1.0);

}