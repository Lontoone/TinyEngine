#version 330 core
uniform sampler2D screenTexture;
uniform vec2 u_texturesize;
uniform vec2 u_texelsize;

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;
float pixelSize = 0.015;

void main() {

    vec2 pixel = vec2(pixelSize);

    vec2 pixelUV = floor(texcoord  / pixel) * pixel;    
    
    color = texture(screenTexture, pixelUV.xy);
    
}
