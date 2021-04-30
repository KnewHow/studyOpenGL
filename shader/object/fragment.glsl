#version 450 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewerPosition;

in vec3 fragPosition;
in vec3 normal;

void main(void) {
    float ambientFactor = 0.1;
    vec3 ambient = ambientFactor * lightColor;

    vec3 ray_dir = normalize(lightPosition - fragPosition);
    vec3 N = normalize(normal);
    float cos_theta = max(dot(ray_dir, N), 0.0);
    vec3 diffuse = cos_theta * lightColor;
    
    float specularFactor = 0.5;
    vec3 view_dir = normalize(viewerPosition - fragPosition);
    vec3 reflect_dir = reflect(-ray_dir, N);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
    vec3 specular = spec * lightColor;

    vec3 result = (specular + ambient + diffuse) * objectColor;
    color = vec4(result, 1.0);
}