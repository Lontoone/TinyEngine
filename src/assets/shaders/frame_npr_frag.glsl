#version 420 core

layout (binding = 0) uniform sampler2D screenTexture;
//layout (binding = 1) uniform sampler2D texture2;
//layout (binding = 2) uniform sampler2D screenTexture;
//vec2 u_textureSize = vec2(1280, 600);  //TODO: Set it via cpu

in vec2 texcoord;
out vec4 color;


uniform int SCR_WIDTH = 1344;
uniform int SCR_HEIGHT = 756;

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord) {
    float w = 1.0 / SCR_WIDTH;
    float h = 1.0 / SCR_HEIGHT;

    n[0] = texture2D(tex, coord + vec2(-w, -h));
    n[1] = texture2D(tex, coord + vec2(0.0, -h));
    n[2] = texture2D(tex, coord + vec2(w, -h));
    n[3] = texture2D(tex, coord + vec2(-w, 0.0));
    n[4] = texture2D(tex, coord);
    n[5] = texture2D(tex, coord + vec2(w, 0.0));
    n[6] = texture2D(tex, coord + vec2(-w, h));
    n[7] = texture2D(tex, coord + vec2(0.0, h));
    n[8] = texture2D(tex, coord + vec2(w, h));
}


void main() {
    /*
    vec4 origin_color = texture(screenTexture, texcoord);    

    vec4 blur_color = vec4(0.0);
    for (float i = -u_blurRadius; i <= u_blurRadius; i++) {
        for (float j = -u_blurRadius; j <= u_blurRadius; j++) {
            vec4 sample_color = texture2D(screenTexture, texcoord + vec2(i, j) * u_texelsize) ;
            float brightness = dot(sample_color.rgb, vec3(0.2126, 0.7152, 0.0722));
            brightness = step(brightness, threshold);
            blur_color += sample_color * brightness;
        }
    }
    blur_color /= (2.0 * u_blurRadius + 1.0) * (2.0 * u_blurRadius + 1.0);    
    color = origin_color + blur_color;
    */
    vec4 n[9];
    make_kernel(n, screenTexture, texcoord);

    vec4 sobel_edge_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
    vec4 sobel_edge_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
    vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    float outline = step(length(sobel.rgb) , 0.8);
    //color = vec4(1.0 - sobel.rgb, 1.0) + texture2D(screenTexture , texcoord) * vec4(sobel.rgb, 1.0);
    vec4 tex_color = texture2D(screenTexture, texcoord);
    tex_color = floor(tex_color * 20) / 20;
    color = vec4(0 , 0 , 0, 1.0) * (1-outline ) + tex_color * (outline);
}