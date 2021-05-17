#version 450 core

out vec4 frag_Color;

uniform vec3 lightPos;
uniform vec3 viewerPos;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_texture;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragLightSpacePos;
} fs_in;


float calculateShadow() {
    vec3 projectCoords = fs_in.fragLightSpacePos.xyz / fs_in.fragLightSpacePos.w;
    projectCoords = projectCoords * 0.5 + 0.5;
    float closestDepth = texture(shadow_texture, projectCoords.xy).r;
    float currentDepth = projectCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 color = texture(diffuse_texture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    vec3 viewerDir = normalize(viewerPos - fs_in.fragPos);

    vec3 ambientColor = 0.3 * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffColor = lightColor * diff;

    vec3 halfVec = normalize(viewerDir + lightDir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 32);
    vec3 specColor = lightColor * spec;

    float shadow = calculateShadow();
    vec3 lightingColor = (ambientColor + (1.0 - shadow) * (specColor + diffColor)) * color;

    //vec3 lightingColor = (diffColor + specColor) * color;

    frag_Color = vec4(lightingColor, 1.0);
}