#version 450 core

out vec4 color;

in vec2 uvCoords;
in vec3 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void) {
    // color = mix(texture(texture1, uvCoords), texture(texture2, uvCoords), 0.2);
    color = mix(texture(texture1, uvCoords), texture(texture2, vec2(1 - uvCoords.x, uvCoords.y)), 0.2); // let is mirror of x-axis
    
}