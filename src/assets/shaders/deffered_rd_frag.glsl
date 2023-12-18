#version 420 core
//layout(binding = 2)  uniform sampler2D texture1;//world pos;
layout(binding = 0) uniform sampler2D texture0;//color_ws_normal;
layout(binding = 1) uniform sampler2D texture1;//color_ws_pos;
layout(binding = 2) uniform sampler2D texture2;//color_ws_normal;
layout(binding = 3) uniform sampler2D texture3;//color_ws_pos;
layout(binding = 4) uniform sampler2D texture4;//color_ambient;
layout(binding = 5) uniform sampler2D texture5;//color_diffuse;
layout(binding = 6) uniform sampler2D texture6;//color_speculr;

layout(binding = 9)  uniform sampler2D u_TEX_SHADOW_MAP;

uniform mat4 u_LIGHT_VP_MATRIX;
uniform vec4 u_LIGHT_WORLD_POS0;
//===================================
//			Deffered Shading
//===================================
/*
layout(location = 1) out vec4 color_tex;
layout(location = 2) out vec4 color_normal;
layout(location = 3) out vec4 color_depth;
layout(location = 4) out vec4 color_depth;
*/

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;

void main() {
	//============================================
	//                 Read Texture
	//============================================
	vec4 diffuse_color = texture(texture5, texcoord);
	vec4 world_normal = texture(texture2, texcoord);
	//============================================

	//vec4 world_pos = texture(texture1, texcoord);
	vec2 scr_pix = vec2(texcoord.x * 1344 , texcoord.y * 756 );
	vec4 world_pos = texelFetch(texture3,ivec2(scr_pix.xy) ,0); // model * aPos

	vec4 light_clip_pos = u_LIGHT_VP_MATRIX * world_pos;
	vec3 light_ndc_pos = light_clip_pos.xyz / light_clip_pos.w;

	vec4 shadow_color = texture(u_TEX_SHADOW_MAP, light_ndc_pos.xy);
	if (light_ndc_pos.z < shadow_color.r) {
		color = vec4(1);
	}
	else {
		//color = shaow_color;
		color = vec4(0);
	}

	vec3 world_light_dir = normalize(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz);
	vec4 diffuse = texture(texture5, texcoord);
	float d_intensity = max(dot(world_normal.xyz, world_light_dir), 0.0f);
	//color = vec4(d_intensity,0,0, 1.0);
	color = diffuse * d_intensity * color;
	/*

	//vec4 world_pos = texture(screenTexture, texcoord);
	//world_pos.w = 1;


	//float shadow_factor = textureProj(u_TEX_SHADOW_MAP, light_clip_pos); 
	vec4 shaow_color = texture(u_TEX_SHADOW_MAP , light_ndc_pos.xy +0.5*0.5);
	//color = shaow_color;
	//color = vec4(light_ndc_pos.xyz+0.5*0.5, 1.0);
	//color = vec4(light_ndc_pos.z,0,0,1.0);
	//color = light_clip_pos;
	if (light_ndc_pos.z < shaow_color.r) {
		color = vec4(0);
	}
	else {
		//color = shaow_color;
		color = vec4(1);
	}*/
	/*
	*/

	//color = vec4(shadow_factor,0,0,1);
	//color = normalize( world_pos)*0.5+0.5;
	//color = diffuse_color* (1-shadow_factor);
	//color = light_clip_pos;

	//color = texture(screenTexture, texcoord);
	//color = texture(screenTexture, vec2(texcoord.x /2 , texcoord.y));
	//color = vec4(texcoord,0,1);
	//color = vec4(1,0,0,1);
	//color = texture(screenTexture, texcoord);
}