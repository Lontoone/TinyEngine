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


void main() {

	color = texture(screenTexture, texcoord) ;
}