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

out vec3 crntPos;
out vec2 texcoord;
out vec3 world_normal;
out mat3 TBN;
out vec3 light_dir;
// Outputs the fragment position of the light
out vec4 world_clip_pos;
out vec4 light_clip_pos;
//out vec3 view_dir;

//out vec4 color;
void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);//+ gl_InstanceID;
	//gl_Position = projection * view * model * vec4(aPos, 1.0) + gl_InstanceID;
	world_normal = aNormal;
	//color =vec4(aColor , 1.0f) ;
	texcoord = aUv;

	vec3 N = mat3(view) * aNormal;
	vec3 T = mat3(view) * aTangent;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);

	light_dir = TBN * sun_postion;
	light_clip_pos = u_LIGHT_VP_MATRIX * model * vec4(aPos, 1.0f);
	world_clip_pos = gl_Position;
}