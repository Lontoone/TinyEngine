#version 420 core
layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D texture2;
uniform vec2 u_mouse_position;
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
//uniform float line_x = 0.5f;

void main() {
	
	vec4 processed_color =  texture(screenTexture, texcoord) ;
	vec4 origin_color = texture(texture2 , texcoord);



	int line_mask = int(abs(texcoord.x - u_mouse_position.x) <= thickness);	

	vec4 combined_color = mix(origin_color, processed_color , texcoord.x > u_mouse_position.x  );
	color = combined_color * (1 - line_mask) + line_color * line_mask;
	//color = processed_color * (1 - line_mask) + line_color * line_mask;
	
	//color = processed_color;
}