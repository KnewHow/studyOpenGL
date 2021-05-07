#version 450 core

uniform sampler2D texture1;

in vec2 texCoords;

out vec4 color;

void main(void) {
   vec4 texColor = texture(texture1, texCoords);
   if(texColor.a < 0.1)
       discard;
    color = texColor;
}