#version 330 core
uniform sampler2D screenTexture;
/*
layout(location = 0) out vec4 color_tex;
layout(location = 1) out vec4 color_normal;
layout(location = 2) out vec4 color_depth;
*/

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;
float thickness = 0.001f;
const vec4 line_color = vec4(255,0,0,1);
uniform float line_x = 0.5f;

void main() {

	int line_mask = int(abs(texcoord.x - line_x) <= thickness);	

	color = texture(screenTexture, texcoord) * (1 - line_mask) + line_color * line_mask;
	
}