#version 330 core

layout (location =0) in vec3 aPos;
//layout (location =1) in vec3 aColor;
//layout (location =2) in vec2 aUv;
layout (location =1) in vec2 aUv;

uniform mat4 transform ; 
uniform mat4 model ; 
uniform mat4 view; 
uniform mat4 projection; 

out vec2 texcoord;
//out vec4 color;
void main(){
	//gl_Position = vec4(aPos , 1.0f) * transform ;
	gl_Position = projection * view * model *vec4(aPos , 1.0);

	//color =vec4(aColor , 1.0f) ;
	texcoord = aUv;
}