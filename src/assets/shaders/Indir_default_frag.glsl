#version 430 core

in vec4 texcoord;

out vec4 frag_color;

uniform sampler2D DIFFUSE;
uniform sampler2D NORMAL;

void main() {
    //frag_color = vec4(texcoord.xy,0,0);
    frag_color = texture(DIFFUSE , texcoord.xy);
    
    if (frag_color.a <0.5) {
        discard;
    }
}