#version 450 core

out vec4 frag_Color;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D displacement_texture;

uniform float height_scale;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewerPos;
    vec3 tangentFragPos;
} fs_in;

vec2 calculateDisplacement(vec2 texCoords, vec3 viewerDir) {
    float height = texture(displacement_texture, texCoords).r;
    vec2 p = viewerDir.xy / viewerDir.z * (height * height_scale);
    return texCoords - p;
}


void main() {
    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    vec3 viewerDir = normalize(fs_in.tangentViewerPos - fs_in.tangentFragPos);
    vec2 texCoords = calculateDisplacement(fs_in.texCoords, viewerDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 color = texture(diffuse_texture, texCoords).rgb;
    vec3 normal = texture(normal_texture, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 ambientColor = 0.1 * color;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffColor = color * diff;

    vec3 halfVec = normalize(viewerDir + lightDir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 32);
    vec3 specColor = color * spec * 0.2;

    vec3 lightingColor = ambientColor + diffColor + specColor;
    frag_Color = vec4(lightingColor, 1.0);
}