#version 330 core
uniform sampler2D screenTexture;
uniform float u_time;

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;
float pixelSize = 0.015;

void main() {

    vec2 offset = 0.01 * vec2(sin(texcoord.x + u_time*5) , sin(texcoord.y + u_time * 5));
    
    color = texture(screenTexture, texcoord + offset);
    
}
