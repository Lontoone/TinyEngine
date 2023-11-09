#version 330 core
uniform sampler2D screenTexture;

in vec2 texcoord;
out vec4 color;
uniform vec2 u_texelsize;
//vec2 u_textureSize = vec2(1280,600);  //TODO: Set it via cpu
float u_blurRadius = 10; //TODO: Set it via cpu



void main() {

	//int line_mask = int(abs(texcoord.x - line_x) <= thickness);	
	//color = texture(screenTexture, texcoord) * (1 - line_mask) + line_color * line_mask;

    /*
    */
    //vec2 texelSize = 1.0 / u_textureSize;
    vec4 blur_color = vec4(0.0);
    for (float i = -u_blurRadius; i <= u_blurRadius; i++) {
        for (float j = -u_blurRadius; j <= u_blurRadius; j++) {
            blur_color += texture2D(screenTexture, texcoord + vec2(i, j) * u_texelsize);
        }
    }
    blur_color /= (2.0 * u_blurRadius + 1.0) * (2.0 * u_blurRadius + 1.0);
    color = blur_color;
   
	
}