#include "FrameBufferDebugger.h"

FramebufferObject* FrameBufferDebugger::gen_frame_object_and_registor(Shader* shader, const GLenum* draw_buffers, int buffer_cnt, int width, int height)
{
	FramebufferObject* fbo = new FramebufferObject(shader, draw_buffers, buffer_cnt, width, height);
	this->m_framebuffers.push_back(fbo);
	fbo->frame_debugger = this;
	return fbo;
}

void FrameBufferDebugger::Draw_Frames_on_Panel()
{
	if (ImGui::Begin("FrameDebugger", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
		for (int i = 0; i < this->m_framebuffers.size(); i++) {
			if(this->m_framebuffers[i]->is_enabled)
				ImGui::Image((void*)this->m_framebuffers[i]->framebuffer_texture[0], ImVec2(100, 100));
				ImGui::SameLine();
		}
	}
}

void FrameBufferDebugger::Trigger_Blits()
{
	
}

void FrameBufferDebugger::Init_Panel(int window_width,int window_height)
{
	//ImGui::Begin("FrameDebugger");
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - window_height));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, window_height));
}

void FrameBufferDebugger::Begin_Panel()
{
	this->is_debugging = ImGui::Begin("FrameDebugger", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
}

void FrameBufferDebugger::End_Panel()
{
	ImGui::End();
}

void FrameBufferDebugger::Draw_Single_Frame(GLuint& texture_id)
{
	if (this->is_debugging) {
		//ImGui::Image((void*)texture_id, ImVec2(100, 100));
		ImGui::Image((void*)texture_id, ImVec2(100, 100) , ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
	}
}

void FrameBufferDebugger::create_hw2_panel(int& setting)
{
	ImGui::Begin("Hw2 Select Effect");
	if (ImGui::Button("None")) {
		setting = 0;
	};
	if (ImGui::Button("Normal")) {
		setting = 1;
	};
	if (ImGui::Button("Abstration")) {
		setting = 2;
	};
	if (ImGui::Button("Water Color")) {
		setting = 3;
	};
	if (ImGui::Button("Magnifier")) {
		setting = 4;
	};
	if (ImGui::Button("Bloom")) {
		setting = 5;
	};

	if (ImGui::Button("Pixelization")) {
		setting = 6;
	};
	if (ImGui::Button("Sine wave distortion")) {
		setting = 7;
	};


	ImGui::End();
}
