#version 450 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};


in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentSpaceViewerPos;
    vec3 tangentSpaceLightDir;
    vec3 tangentSpaceFragPos;
} fs_in;

uniform Material material;

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionLight directionLight;


out vec4 frag_Color;

vec3 calculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewerDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));

    vec3 ray_dir = normalize(-fs_in.tangentSpaceLightDir);
    float diff = max(dot(ray_dir, normal), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, fs_in.texCoords)));
    
    vec3 halfVec = normalize(viewerDir + ray_dir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 32);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, fs_in.texCoords)));

    return ambient + diffuse + specular;
}

void main() {
    vec3 res = vec3(0.0);
    vec3 normal = texture(material.texture_normal1, fs_in.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 viewerDir = normalize(fs_in.tangentSpaceViewerPos - fs_in.tangentSpaceFragPos);

    res += calculateDirectionLight(directionLight, normal, viewerDir);
    // res = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    frag_Color = vec4(res, 1.0);
}