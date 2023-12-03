#version 460 core

in vec4 texcoord;
flat in int draw_id;

out vec4 frag_color;

uniform sampler2D DIFFUSE;
//uniform sampler2D NORMAL;
//uniform sampler2DArray  albedoTextureArray; 
//uniform sampler2D  albedoTextureArray;

void main() {
    frag_color = texture(DIFFUSE, texcoord.xy) ;
    //frag_color = vec4(texcoord.xy,0,0);
    //vec3 uv = vec3(texcoord.xy, 0);
    //frag_color = texture(albedoTextureArray, uv);
    //frag_color = texture(albedoTextureArray, texcoord.xy);
    if (frag_color.a <0.5) {
        discard;
    }
    /*
    */
}