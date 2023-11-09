#pragma once
#ifndef FRAME_BUFFER_DEBUGGER
#define FRAME_BUFFER_DEBUGGER
#include "FrameBufferObject.h"
#include <imgui.h>
#include <vector>
class FramebufferObject;

using namespace std;
class FrameBufferDebugger
{
public:
	vector<FramebufferObject*> m_framebuffers;
	FramebufferObject* gen_frame_object_and_registor(Shader* shader, const GLenum* draw_buffers, int buffer_cnt, int width, int height);
	void Draw_Frames_on_Panel();
	void Trigger_Blits();
	void Init_Panel(int window_width, int window_height);
	void Begin_Panel();
	void End_Panel();
	void Draw_Single_Frame(GLuint& texture_id);

	//================= hw 2 ======================
	void create_hw2_panel(int& setting);
private:
	bool is_debugging = true;
};

#endif // !FRAME_BUFFER_DEBUGGER
