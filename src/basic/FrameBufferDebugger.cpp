#include "FrameBufferDebugger.h"

FramebufferObject* FrameBufferDebugger::gen_frame_object_and_registor(Shader* shader, const GLenum* draw_buffers, int buffer_cnt, int width, int height)
{
	FramebufferObject* fbo = new FramebufferObject(shader, draw_buffers, buffer_cnt, width, height);
	this->m_framebuffers.push_back(fbo);
	return fbo;
}

void FrameBufferDebugger::Draw_Frames()
{
	for (int i = 0; i < this->m_framebuffers.size(); i++) {
		ImGui::Image((void*)this->m_framebuffers[i]->framebuffer_texture[0], ImVec2(100, 100));
	
	}
}

void FrameBufferDebugger::Init_Panel()
{
	ImGui::Begin("FrameDebugger");
}

void FrameBufferDebugger::End_Panel()
{
	ImGui::End();
}
