#version 420 core

layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D texture2;
//layout (binding = 2) uniform sampler2D screenTexture;

in vec2 texcoord;
out vec4 color;


void main() {

	//vec4 blur_color = texture(screenTexture, texcoord);
	vec4 noise_color = texture(texture2, texcoord);
	//vec4 origin_color = texture(screenTexture, texcoord);
	
	vec2 distorted_uv = texcoord  + noise_color.xy*0.05;

	color = texture(screenTexture, distorted_uv) ;
	//color = blur_color - noise_color;
	//color = noise_color;
}