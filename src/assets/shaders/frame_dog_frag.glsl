#version 420 core
layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D qaoTexture;
//vec2 u_textureSize = vec2(1280, 600);  //TODO: Set it via cpu
uniform vec2 u_texelsize;
in vec2 texcoord;
out vec4 color;
//float u_radious = 0.05;


vec4 gaussianBlur(sampler2D tex, vec2 texSize, float radius) {
    //vec2 texelSize = 1.0 / texSize;
    vec4 gcolor = vec4(0.0);
    for (float i = -radius; i <= radius; i++) {
        for (float j = -radius; j <= radius; j++) {
            gcolor += texture2D(tex, texcoord + vec2(i, j) * texSize);
        }
    }
    gcolor /= (2.0 * radius + 1.0) * (2.0 * radius + 1.0);
    return gcolor;
}

void main() {
        
    vec4 texcolor = texture2D(screenTexture, texcoord);
    vec4 quacolor = texture2D(qaoTexture, texcoord);

    vec4 color1 = gaussianBlur(screenTexture, u_texelsize, 1);
    vec4 color2 = gaussianBlur(screenTexture, u_texelsize, 3);
    float mask = 1- step(length(color1 - color2) , 0.05);
    color = quacolor *(1-mask) + vec4(0, 0, 0, 1) * (mask);

}