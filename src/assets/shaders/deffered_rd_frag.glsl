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
uniform vec3 u_CAM_POS;
uniform mat4 view;
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

float ia = 0.1; //ToDo: ambient term
float id = 0.7;
float is = 0.7;

void main() {
	//============================================
	//                 Read Texture
	//============================================
	vec4 diffuse_color = texture(texture5, texcoord);
	vec4 world_normal = texture(texture2, texcoord);
	vec4 ambient_color = texture(texture4, texcoord);
	vec4 specular_color = texture(texture6, texcoord);  // rgb = color tint , a = shininess
	//============================================

	//==============================================
	//			Main Light Shadow
	//==============================================
	vec2 scr_pix = vec2(texcoord.x * 1344 , texcoord.y * 756 );
	vec4 world_pos = texelFetch(texture3,ivec2(scr_pix.xy) ,0); // model * aPos

	vec4 light_clip_pos = u_LIGHT_VP_MATRIX * world_pos;
	vec3 light_ndc_pos = light_clip_pos.xyz / light_clip_pos.w;

	vec4 shadow_color = texture(u_TEX_SHADOW_MAP, light_ndc_pos.xy);
	float shadow_term = 0;
	if (light_ndc_pos.z < shadow_color.r) {
		shadow_term = 1;
	}
	else {		
		shadow_term = 0;
	}
	//shadow_term = shadow_term * 0.5 + 0.5;

	//==============================================
	//			Diffuse
	//==============================================
	vec3 world_light_dir = normalize(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz);
	vec4 diffuse = texture(texture5, texcoord);
	float d_intensity = max(dot(world_normal.xyz, world_light_dir), 0.0f);
	//color = vec4(d_intensity,0,0, 1.0);
	
	//==============================================
	//			Specular
	//==============================================
	//vec3 world_view_dir = normalize(u_CAM_POS - world_pos.xyz);
	vec3 view_normal = mat3(view) * world_normal.xyz;  //N
	vec3 view_pos = mat3(view) * (u_CAM_POS - world_pos.xyz);					//P
	vec3 L = mat3(view) *(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz);  // L
	vec3 V = -view_pos.xyz;  //V

	L = normalize(L);
	V = normalize(V);
	vec3 R = reflect(-L , view_normal);
	view_pos = normalize(view_pos);
	float specular_term = pow(max(dot(R, V), 0.0f), specular_color.a);

	//color = (ambient_color * ia + vec4(1) * d_intensity + specular_color * specular_term) * diffuse * shadow_term; 
	color = ambient_color * ia * diffuse +
			diffuse * d_intensity * shadow_term * id +
			specular_color * specular_term * shadow_term * is * diffuse;
	/*
	*/
	/*
	color = specular_color * specular_term;
	*/
	//color = vec4(R, 1.0);
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