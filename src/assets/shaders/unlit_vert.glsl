#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUv;
uniform mat4 MATRIX_VP;

out vec2 texcoord;

void main(){
	
	//gl_Position = projection * view * model *vec4(aPos , 1.0);	
	gl_Position = MATRIX_VP * vec4(aPos, 1.0);
	
	texcoord = aUv;

}