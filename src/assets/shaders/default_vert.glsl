#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

uniform mat4 transform ; 
uniform mat4 model ; 
uniform mat4 view; 
uniform mat4 projection; 
uniform mat4 MATRIX_VP;
//uniform vec3 sun_postion;
// 
vec3 sun_postion = vec3(0,1,0);

// Imports the light matrix
uniform mat4 u_LIGHT_VP_MATRIX;
uniform vec4 u_LIGHT_WORLD_POS0;
uniform vec4 u_CAM_POS;


out vec2 texcoord;
out vec3 world_normal;
out vec3 world_pos;
out mat3 TBN;
out vec3 world_light_dir;
out vec3 world_view_dir;
out vec3 world_half_dir;

// Outputs the fragment position of the light
out vec4 world_clip_pos;
out vec4 light_clip_pos;
//out vec3 tng_light_dir;  // light direction in tangent space
//out vec3 tng_view_dir;  // light direction in tangent space
//out vec3 view_dir;

//out vec4 color;
void main(){

	// ========================
	//        Self Info
	// ========================
	world_pos = (model * vec4(aPos, 1.0)).xyz;
	gl_Position = projection * view * model * vec4(aPos, 1.0);//+ gl_InstanceID;
	
	world_normal = (model * vec4(aNormal,1.0)).xyz;
	texcoord = aUv;

	//mat4 mv = view * model;
	vec3 N = mat3(model) * aNormal;
	vec3 T = mat3(model) * aTangent;
	//T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);


	// ========================
	//      World Light Info
	// ========================
	/*
	*/
	world_light_dir = normalize(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz) ;
	world_view_dir =  normalize(u_CAM_POS.xyz - world_pos.xyz);
	world_half_dir =  normalize(world_light_dir.xyz + world_view_dir.xyz);
	light_clip_pos = u_LIGHT_VP_MATRIX * model * vec4(aPos, 1.0f);
	world_clip_pos = gl_Position;

	// ========================
	//      Light space
	// ========================
	/*
	tng_light_dir = normalize(
		vec3(
			dot(world_light_dir, T),
			dot(world_light_dir, B),
			dot(world_light_dir, N)
		));

	tng_view_dir = normalize(
		vec3(
			dot(world_view_dir, T),
			dot(world_view_dir, B),
			dot(world_view_dir, N)
		));
	*/

}