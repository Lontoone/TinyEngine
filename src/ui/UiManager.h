#pragma once

#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <functional>
#include <GameObject.h>

using namespace std;
using namespace ImGui;
class UiManager
{
public:
	UiManager();
	//~UiManager();

	//******** Registored ui command **********	
	typedef function<bool()> t_draw_cmd;
	vector<t_draw_cmd> m_menu_cmds;
	//**********************

	void init_imgui(GLFWwindow* window);
	void new_frame();
	void destory();

	void create_menubar();
	void create_sceneNgame_window(uint &scene_buffer_id , uint &game_buffer_id);
	void render_ui();

	void create_window(const char* _window_name);
	void create_hierarchy_window(vector<GameObject*>&  objs);
	void end_create();

private:

};


#endif // !UI_MANAGER_H

