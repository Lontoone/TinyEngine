#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 inTexCoords;

out vec2 texcoord;

void main() {	
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	texcoord = inTexCoords;
}