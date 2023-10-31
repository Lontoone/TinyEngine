#pragma once
#ifndef UI_ITEM_H
#define UI_ITEM_H
#include <vector>
#include <functional>

using namespace std;
class UiItem
{
public:
	/*
	UiItem();
	~UiItem();
	*/

	typedef function<void()> t_draw_cmd;
	vector<t_draw_cmd> m_draw_cmds;

	virtual void init_ui_item() {};
	void add_draw_item(t_draw_cmd& _draw_cmd);
	void run_all_draw_cmd();

private:
	
};


#endif // !UI_UNIT_H
