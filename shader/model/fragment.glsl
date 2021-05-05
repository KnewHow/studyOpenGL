#version 450 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform vec3 viewerPosition;

in vec2 texCoords;
in vec3 inFragPosition;
in vec3 inNormal;

out vec4 color;

vec3 calculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewerDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));

    vec3 ray_dir = normalize(-light.direction);
    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texCoords)));
    
    vec3 reflect_dir = reflect(-ray_dir, normal);
    float spec = pow(max(dot(viewerDir, reflect_dir), 0.0), 32);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texCoords)));

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 fragPosition, vec3 normal, vec3 viewerDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));

    vec3 ray_dir = normalize(light.position - fragPosition);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texCoords)));
    
    vec3 reflect_dir = reflect(-ray_dir, normal);
    float spec = pow(max(dot(viewerDir, reflect_dir), 0.0), 32);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texCoords)));

   return (specular + ambient + diffuse) * attenuation;
}

void main() {
    vec3 res = vec3(0.0);
    vec3 normal = normalize(inNormal);
    vec3 viewerDir = normalize(viewerPosition - inFragPosition);

    //res += calculateDirectionLight(directionLight, normal, viewerDir);
    res += calculatePointLight(pointLight, inFragPosition, normal, viewerDir);
    color = vec4(res, 1.0);
}