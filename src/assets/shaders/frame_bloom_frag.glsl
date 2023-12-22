#version 420 core

layout (binding = 0) uniform sampler2D screenTexture;

uniform vec2 u_texelsize;
float u_blurRadius = 5; //TODO: Set it via cpu

in vec2 texcoord;
out vec4 color;

float threshold = 0.95;


void main() {
    
    vec4 origin_color = texture(screenTexture, texcoord);    

    vec4 blur_color = vec4(0.0);
    float _cnt = 1;
    for (float i = -u_blurRadius; i <= u_blurRadius; i++) {
        for (float j = -u_blurRadius; j <= u_blurRadius; j++) {
            vec4 sample_color = texture2D(screenTexture, texcoord + vec2(i, j) * u_texelsize) ;
            float brightness = dot(sample_color.rgb, vec3(0.2126, 0.7152, 0.0722));
            brightness = 1-step(brightness, threshold);
            //blur_color += sample_color * brightness * 0.25;
            blur_color += brightness / _cnt;
        }
        _cnt++;
    }
    blur_color /= (2.0 * u_blurRadius + 1.0) * (2.0 * u_blurRadius + 1.0);    
    color = origin_color + blur_color;
    
}