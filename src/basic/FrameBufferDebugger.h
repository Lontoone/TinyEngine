#pragma once
#ifndef FRAME_BUFFER_DEBUGGER
#define FRAME_BUFFER_DEBUGGER
#include "FrameBufferObject.h"
#include <imgui.h>
#include <vector>

using namespace std;
class FrameBufferDebugger
{
public:
	vector<FramebufferObject*> m_framebuffers;
	FramebufferObject* gen_frame_object_and_registor(Shader* shader, const GLenum* draw_buffers, int buffer_cnt, int width, int height);
	void Draw_Frames();
	void Init_Panel();
	void End_Panel();
private:

};

#endif // !FRAME_BUFFER_DEBUGGER
