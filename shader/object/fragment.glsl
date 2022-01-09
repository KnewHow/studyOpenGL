#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight {
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

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // light intensity term
    float linear;
    float quadratic;
};

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionLight directionLight;

uniform Material material;

uniform vec3 viewerPosition;

in vec3 inFragPosition;
in vec3 inNormal;
in vec2 texCoords;

out vec4 color;

vec3 calculateSpotLight(SpotLight light, vec3 fragPosition, vec3 normal, vec3 viewerDir) {
    vec3 ray_dir = normalize(light.position - fragPosition);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    float theta = dot(ray_dir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));
    diffuse *= intensity;
    
    vec3 reflect_dir = reflect(-ray_dir, normal);
    float spec = pow(max(dot(viewerDir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));
    specular *= intensity;

    return (specular + ambient + diffuse) * attenuation;
}

vec3 calculatePointLight(PointLight light, vec3 fragPosition, vec3 normal, vec3 viewerDir) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    vec3 ray_dir = normalize(light.position - fragPosition);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));
    
    vec3 reflect_dir = reflect(-ray_dir, normal);
    float spec = pow(max(dot(viewerDir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));

   return (specular + ambient + diffuse) * attenuation;
}

vec3 calculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewerDir) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    vec3 ray_dir = normalize(-light.direction);
    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texCoords)));
    
    vec3 reflect_dir = reflect(-ray_dir, normal);
    float spec = pow(max(dot(viewerDir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoords)));

    return specular + ambient + diffuse;
}

void main(void) {
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(inNormal);
    vec3 viewDir = normalize(viewerPosition - inFragPosition);
    result += calculateSpotLight(spotLight, inFragPosition, normal, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calculatePointLight(pointLights[i], inFragPosition, normal, viewDir);
    }
    result += calculateDirectionLight(directionLight, normal, viewDir);
    color = vec4(result, 1.0);
}