#version 420 core

layout (binding = 0) uniform sampler2D screenTexture;
//layout (binding = 1) uniform sampler2D texture2;
//layout (binding = 2) uniform sampler2D screenTexture;
//vec2 u_textureSize = vec2(1280, 600);  //TODO: Set it via cpu
uniform vec2 u_texelsize;
float u_blurRadius = 5; //TODO: Set it via cpu

in vec2 texcoord;
out vec4 color;

float threshold = 0.8;


void main() {

    vec4 origin_color = texture(screenTexture, texcoord);
    

    //vec2 texelSize = 1.0 / u_textureSize;

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
    //color = blur_color;
    color = origin_color + blur_color;
	//color = vec4(brightness);
}