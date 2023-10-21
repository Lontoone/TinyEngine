#pragma once
#ifndef UI_PANEL_H
#define UI_PANEL_H
#include "UiItem.h"

// For generating random ID////
//#include <iostream>
//#include <unordered_map>
///////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
using namespace std;
using namespace ImGui;
class UiPanel
{
public:	
	//UiPanel(uint32_t* cmp_id);
	//~UiPanel();

	const char* m_target_comp_id;
	typedef function<bool()> t_draw_cmd;
	vector<t_draw_cmd> m_draw_cmds;
	//void add_draw_item(t_draw_cmd& _draw_cmd);

	void add_draw_item(function<bool()> _draw_cmd);
	void add_drop_down_menu(vector<string>& _ops, void(*func)(string));
	void draw_ui_panel();
	void create_panel(const char* window_name);
	void panel_begin();
	void panel_end();
	
	//void add_item(UiItem * item);

	vector<UiItem*> m_items;
	const char* m_panel_names ="Default window";

private:
	//std::string generate_random_id(size_t length);
};

#endif // !UI_PANEL_H
