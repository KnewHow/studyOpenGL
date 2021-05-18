#version 450 core

uniform vec3 lightPos;
uniform float z_far;

in vec4 fragPos;

void main() {
    float lightDistance = length(fragPos.xyz - lightPos);
    lightDistance = lightDistance / z_far;
    gl_FragDepth = lightDistance;
}