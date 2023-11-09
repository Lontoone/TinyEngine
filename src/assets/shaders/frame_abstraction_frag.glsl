#version 330 core
uniform sampler2D screenTexture;
//vec2 u_textureSize = vec2(1280, 600);  //TODO: Set it via cpu
uniform vec2 u_texelsize;
in vec2 texcoord;
out vec4 color;
float u_radious = 5;
float u_levels = 50;
vec4 outline_color = vec4(0, 0, 0, 1);


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
    //Blur
    vec4 blur_color = gaussianBlur(screenTexture, u_texelsize, 1);
    
    //Quan
    vec4 mix_quan_color = round(blur_color * u_levels) / u_levels;

    //DOG
    //vec4 color1 = gaussianBlur(screenTexture, u_textureSize, 2);
    vec4 color2 = gaussianBlur(screenTexture, u_texelsize, 3);
    vec4 dog_color = blur_color - color2;

    float outline_mask = step( length(dog_color), 0.1);

    color = mix(outline_color, mix_quan_color, outline_mask);

}