#version 430 core
out vec4 FragColor;
in vec2 texcoord;

//in vec4 color;
uniform mat4 model;
uniform mat4 u_LIGHT_VP_MATRIX;
uniform vec4 u_LIGHT_WORLD_POS0;

in vec3 world_pos;
//in vec3 world_normal;
in vec3 tng_normal;
in mat3 TBN;
in mat3 inv_TBN;
in vec3 light_dir;
in vec4 world_clip_pos;
in vec4 light_clip_pos;
/*
vec3 tng_light_dir;
vec3 tng_view_dir; 
*/
in vec3 world_light_dir;
in vec3 world_view_dir;
in vec3 world_half_dir;


//layout(location = 0) out vec4 color_tex;
layout(location = 1) out vec4 color_ws_normal;
layout(location = 2) out vec4 color_ws_pos;
layout(location = 3) out vec4 color_ambient;
layout(location = 4) out vec4 color_diffuse;
layout(location = 5) out vec4 color_speculr;


uniform sampler2D DIFFUSE;
uniform sampler2D NORMAL;
uniform bool NO_DIFFUSE = false;
uniform bool NO_NORMAL	= false;
//uniform bool u_write_gbuffer;

uniform vec3 u_MAT_PARA_ID;
uniform vec3 u_MAT_PARA_IS;
uniform vec3 u_MAT_PARA_IA;
uniform float u_MAT_PARA_SN;

//uniform sampler2D u_TEX_SHADOW_MAP;
uniform sampler2DShadow u_TEX_SHADOW_MAP;
// Gets the position of the camera from the main function
uniform vec3 u_CAM_POS;

vec3 lightPos = vec3(0, 1, 0);
float specAmount = 0.5;
vec4 specularLight = vec4(1,1,1,1);

vec4 ia = vec4(u_MAT_PARA_IA ,1);
vec4 id = vec4(u_MAT_PARA_ID, 1);
vec4 is = vec4(u_MAT_PARA_IS, u_MAT_PARA_SN);

float near = 1;
float far = 100.0f;
float linear_depth(float d) {
	d = (2 * near * far) / (far + near - (d * 2.0 - 1.0) * (far - near));
	return d / far;
}



void main(){

	vec3 world_normal = normalize( TBN * tng_normal);
	if (!NO_NORMAL) {
		world_normal = normalize((texture(NORMAL, texcoord).xyz * 2.0 - vec3(1.0)) );
		world_normal = normalize(inv_TBN * world_normal);
	}

	//===================================
	//			Deffered Shading
	//===================================
	if (!NO_DIFFUSE) {
		color_diffuse = texture(DIFFUSE, texcoord);
	}
	else {
		color_diffuse = id;  // default diffuse color
	}
	if (color_diffuse.a < 0.05) {
		discard;
	}


	//color_tex = FragColor;
	color_ws_pos = vec4(world_pos.xyz, 1.0);
	color_ws_normal = vec4(world_normal, 1.0);
	//color_ws_pos = normalize( vec4(world_pos.xyz, 1.0))*0.5+0.5;
	//color_ws_pos	= normalize( vec4(world_pos.xyz, 1.0))*0.5+0.5;
	color_ambient = ia;
	color_speculr = is; 
	FragColor = color_ws_pos;

}