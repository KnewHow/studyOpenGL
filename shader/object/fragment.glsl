#version 450 core

out vec4 frag_Color;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewerPos;
    vec3 tangentFragPos;
} fs_in;


void main() {
    vec3 color = texture(diffuse_texture, fs_in.texCoords).rgb;
    vec3 normal = texture(normal_texture, fs_in.texCoords).rgb;
    vec3 lightColor = vec3(0.6);
    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    vec3 viewerDir = normalize(fs_in.tangentViewerPos - fs_in.tangentFragPos);

    vec3 ambientColor = 0.15 * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffColor = lightColor * diff;

    vec3 halfVec = normalize(viewerDir + lightDir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 64);
    vec3 specColor = lightColor * spec;

    vec3 lightingColor = (ambientColor + specColor + diffColor)) * color
    frag_Color = vec4(lightingColor, 1.0);
}