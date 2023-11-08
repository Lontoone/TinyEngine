#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

uniform mat4 transform ; 
uniform mat4 model ; 
uniform mat4 view; 
uniform mat4 projection; 
uniform vec3 sun_postion;

out vec2 texcoord;
out vec3 world_normal;
out mat3 TBN;
out vec3 light_dir;
//out vec3 view_dir;

//out vec4 color;
void main(){
	//gl_Position = vec4(aPos , 1.0f) * transform ;
	gl_Position = projection * view * model *vec4(aPos , 1.0);
	//world_normal = (projection * view * model * vec4(aNormal, 1.0)).xyz;
	world_normal = aNormal;

	//color =vec4(aColor , 1.0f) ;
	texcoord = aUv;

	vec3 N = mat3(model) * aNormal;
	vec3 T = mat3(model) * aTangent;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);

	light_dir = TBN * sun_postion;

}