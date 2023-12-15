#version 330 core
uniform sampler2D screenTexture;
//uniform sampler2D u_TEX_SHADOW_MAP;
/*
layout(location = 0) out vec4 color_tex;
layout(location = 1) out vec4 color_normal;
layout(location = 2) out vec4 color_depth;
*/

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;

float near = 1;
float far = 50;

void main() {
	/*
	float depth = texture2D(u_TEX_SHADOW_MAP, texcoord).r*2-1;
	//Linear depth
	depth = (2 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
	color = vec4(vec3(depth/far ), 1.0);
	*/

	color = texture(screenTexture, texcoord) ;
	//color = texture(screenTexture, vec2(texcoord.x /2 , texcoord.y));
	//color = vec4(texcoord,0,1);
	//color = vec4(1,0,0,1);
}