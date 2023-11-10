#pragma once
#ifndef RUNTIME_SHADER_EDITOR_H
#define RUNTIME_SHADER_EDITOR_H

#include <imgui.h>
#include <Fastsetup.h>
#include <algorithm>
using namespace std;
class ShaderEditor
{
public:
	ShaderEditor();
	char vert_buffer[1024 * 16] =
		"	#version 330 core\n\
			layout(location = 0) in vec2 aPos;\n\
			layout(location = 1) in vec2 inTexCoords;\n\
			out vec2 texcoord;\n\
			void main() {	\n\
				gl_Position = vec4(aPos, 0.0f, 1.0f);\n\
				texcoord = inTexCoords;\n\
			}\0";
	char frag_buffer[1024 * 16] = 
		"\
#version 330 core								\n\
uniform sampler2D screenTexture;				\n\
															\n\
in vec2 texcoord;								\n\
out vec4 color;									\n\
void main() {									\n\
	color = texture(screenTexture, texcoord);	\n\
}												\0";
	Shader shader;

	void begin_panel();
	void compile_shader(bool  is_init);
	
private:

};


#endif // !RUNTIME_SHADER_EDITOR_H
