#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // light intensity term
    float linear;
    float quadratic;
};

uniform Light light;
uniform Material material;

out vec4 color;

uniform vec3 viewerPosition;

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoords;

void main(void) {
    vec3 result;
    vec3 ray_dir = normalize(light.position - fragPosition);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    float theta = dot(ray_dir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 N = normalize(normal);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    float diff = max(dot(ray_dir, N), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));
    diffuse *= intensity;
    
    vec3 view_dir = normalize(viewerPosition - fragPosition);
    vec3 reflect_dir = reflect(-ray_dir, N);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));
    specular *= intensity;

    result = (specular + ambient + diffuse);

    result *= attenuation;
    color = vec4(result, 1.0);
}