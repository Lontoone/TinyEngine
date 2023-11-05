#version 330 core
uniform sampler2D screenTexture;

in vec2 texcoord;
out vec4 color;

vec2 u_textureSize = vec2(1280,600);  //TODO: Set it via cpu
float u_blurRadius = 1; //TODO: Set it via cpu

vec4 gaussianBlur(sampler2D tex, vec2 texSize, float radius , vec2 texcoord) {
    vec2 texelSize = 1.0 / texSize;
    vec4 blur_color = vec4(0.0);
    for (float i = -radius; i <= radius; i++) {
        for (float j = -radius; j <= radius; j++) {
            blur_color += texture2D(tex, texcoord + vec2(i, j) * texelSize);
        }
    }
    blur_color /= (2.0 * radius + 1.0) * (2.0 * radius + 1.0);
    return blur_color;
}

void main() {

	//int line_mask = int(abs(texcoord.x - line_x) <= thickness);	
	//color = texture(screenTexture, texcoord) * (1 - line_mask) + line_color * line_mask;

    /*
    */
    vec2 texelSize = 1.0 / u_textureSize;
    vec4 blur_color = vec4(0.0);
    for (float i = -u_blurRadius; i <= u_blurRadius; i++) {
        for (float j = -u_blurRadius; j <= u_blurRadius; j++) {
            blur_color += texture2D(screenTexture, texcoord + vec2(i, j) * texelSize);
        }
    }
    blur_color /= (2.0 * u_blurRadius + 1.0) * (2.0 * u_blurRadius + 1.0);
    color = blur_color;
   
	
}