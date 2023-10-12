#pragma once

#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;
using namespace ImGui;
class UiManager
{
public:
	UiManager();
	//~UiManager();

	void init_imgui(GLFWwindow* window);
	void new_frame();
	void destory();

	void _test();


private:

};


#endif // !UI_MANAGER_H

