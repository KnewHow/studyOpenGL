#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 worldPosition;
} gs_in[];

out vec2 outTexCoords;

uniform float time;
uniform mat4 view;
uniform mat4 projection;

vec3 getNormal() {
    vec3 a = gs_in[1].worldPosition - gs_in[0].worldPosition;
    vec3 b = gs_in[2].worldPosition - gs_in[0].worldPosition;
    return normalize(cross(a, b));
}

vec3 explode(vec3 position, vec3 normal) {
    float magnitude = 0.1;
    vec3 direction = normal * time * magnitude;
    return position + direction;
}

void main() {
    vec3 normal = getNormal();
    vec3 newPosition;
    
    newPosition = explode(gs_in[0].worldPosition, normal);
    gl_Position = projection * view * vec4(newPosition, 1.0);
    outTexCoords = gs_in[0].texCoords;
    EmitVertex();
    
    newPosition = explode(gs_in[1].worldPosition, normal);
    gl_Position = projection * view * vec4(newPosition, 1.0);
    outTexCoords = gs_in[1].texCoords;
    EmitVertex();
    
    newPosition = explode(gs_in[2].worldPosition, normal);
    gl_Position = projection * view * vec4(newPosition, 1.0);
    outTexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}

