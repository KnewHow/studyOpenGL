#version 450 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

out vec4 color;

uniform vec3 viewerPosition;

in vec3 fragPosition;
in vec3 normal;

void main(void) {
    vec3 ambient = light.ambient * material.ambient;

    vec3 ray_dir = normalize(light.position - fragPosition);
    vec3 N = normalize(normal);
    float diff = max(dot(ray_dir, N), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 view_dir = normalize(viewerPosition - fragPosition);
    vec3 reflect_dir = reflect(-ray_dir, N);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (material.specular * spec);

    vec3 result = specular + ambient + diffuse;
    color = vec4(result, 1.0);
}