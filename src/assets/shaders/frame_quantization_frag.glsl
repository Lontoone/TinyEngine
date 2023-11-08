#version 330 core
uniform sampler2D screenTexture;

in vec2 texcoord;
out vec4 color;
float u_levels = 50;

void main() {
        
    vec4 texcolor = texture2D(screenTexture, texcoord);

    // Reduce the color space
    color.rgb = round(texcolor.rgb * u_levels) / u_levels;

}