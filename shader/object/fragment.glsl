#version 450 core

out vec4 frag_Color;

uniform vec3 lightPos;
uniform vec3 viewerPos;
uniform float z_far;

uniform sampler2D diffuse_texture;
uniform samplerCube shadow_cube;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;


float calculateShadow(vec3 fragPos, vec3 normal, vec3 lightDir) {
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float closestDepth = texture(shadow_cube, fragToLight).r * z_far;
    float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);
    
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // return shadow;
    
    //pcf
    float shadow = 0.0;
    float offset = 0.1;
    float samples = 4.0;
    for(float x = -offset; x < offset; x += (2 * offset / samples)) {
        for(float y = -offset; y < offset; y += (2 * offset / samples)) {
            for(float z = -offset; z < offset; z += (2 * offset / samples)) {
                vec3 samplerPos = fragToLight + vec3(x, y, z);
                float samplerDepth = texture(shadow_cube, samplerPos).r * z_far;
                shadow += currentDepth - bias > samplerDepth ? 1.0 : 0.0;
            }
        }   
    }
    return shadow / (samples * samples * samples);
}

void main() {
    vec3 color = texture(diffuse_texture, fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightColor = vec3(0.6);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    vec3 viewerDir = normalize(viewerPos - fs_in.fragPos);

    vec3 ambientColor = 0.3 * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffColor = lightColor * diff;

    vec3 halfVec = normalize(viewerDir + lightDir);
    float spec = pow(max(dot(normal, halfVec), 0.0), 64);
    vec3 specColor = lightColor * spec;

    float shadow = calculateShadow(fs_in.fragPos, normal, lightDir);
    vec3 lightingColor = (ambientColor + (1.0 - shadow) * (specColor + diffColor)) * color;
    frag_Color = vec4(lightingColor, 1.0);
}