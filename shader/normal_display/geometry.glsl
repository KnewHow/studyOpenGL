#version 450 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform mat4 projection;

in VS_OUT {
    vec3 normal;
} gs_in[];

void genNormal(int index) {
    const float MAGNITUDE = 0.4;
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + MAGNITUDE * vec4(gs_in[index].normal, 0.0));
    EmitVertex();
    EndPrimitive();
}

void main() {
    for(int i = 0; i < 3; i++) {
        genNormal(i);
    }
}

