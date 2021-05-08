#version 450 core

in vec2 texCoords;
uniform sampler2D screenTexture;

out vec4 color;

void main() {
   //inversion
   // color = vec4(vec3(1 - texture(screenTexture, texCoords)), 1.0); 

      // grayscale
//    vec3 rgb = texture(screenTexture, texCoords).rgb;
//    // float average = (rgb.r + rgb.g + rgb.b) / 3.0; // normal average
//    float average = 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b; // average with weight, make it statify human eye
//    color = vec4(average, average, average, 1.0);


   
    const float offset = 1.0 / 300.0;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    //  kernel matrix
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    // blur matrix
    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );

    // edge detection
    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = texture(screenTexture, texCoords.st + offsets[i]).rgb;
    }
    vec3 res = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        res += kernel[i] * sampleTex[i];
    }
    color = vec4(res, 1.0);

}