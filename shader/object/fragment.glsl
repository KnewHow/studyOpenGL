#version 450 core

out vec4 color;

uniform sampler2D texture1;
uniform samplerCube cubemapTex;
uniform vec3 viewerPos;

in vec3 fragPosition;
in vec3 normal;


void main(void) {
    vec3 I = normalize(fragPosition - viewerPos);
    vec3 R = reflect(I, normalize(normal));
    color = vec4(texture(cubemapTex, R).rgb, 1.0);
}