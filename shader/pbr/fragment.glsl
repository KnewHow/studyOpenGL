#version 450 core

out frag_Color;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

uniform vec3 lightPostions[4];
uniform vec3 lightColors[4];

uniform viewerPos;

const float PI = 3.14159265359;

in VS_OUT {
    vec3 worldPos;
    vec3 normal;
    vec3 texcoords;
} fs_in;

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(normal_map, fs_in.texcoords).xyz * 2.0 - 1.0;
    vec3 Q1 = dFdx(fs_in.worldPos);
    vec3 Q2 = dFdy(fs_in.worldPos);
    vec2 st1 = dFdx(fs_in.texcoords);
    vec2 st2 = dFdy(fs_in.texcoords);

    vec3 N = normalize(fs_in.normal);
    vec3 T = normalize(Q1 * st2.y - Q2 * st1.y);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

float NDFGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom =NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(vec3 H, vec3 V, vec3 F0) {
    return F0 + (1 - F0) * pow(max(1 - max(dot(H, V), 0.0), 0.0), 5.0);
}

void main() {
    vec3 albedo = pow(texture(albedo_map, fs_in.texcoords).rgb, vec3(2.2));
    float metallic = texture(metallic_map, fs_in.texcoords).r;
    float roughness = texture(roughness_map, fs_in.texcoords).r;
    float ao = texture(ao_map, fs_in.roughness);

    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewerPos - fs_in.worldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; i++){
        vec3 L = normalize(lightPostions[i] - fs_in.worldPos);
        vec3 H = normalize(L + V);
        float distance = length(lightPostions[i] - fs_in.worldPos);
        float attenunation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenunation;

        float NDF = NDFGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(H, V, F0);
       
        vec3  numerator = NDF * G * F;
        float denominator =  4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;

        vec3 Ks = F;
        vec3 Kd = vec3(1.0) - Ks;
        Kd *= (1 - metallic);

        float NdotL = max(dot(N, L), 0.0);

        Lo += (Kd * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = Lo + ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    frag_Color = vec4(color, 1.0);
}