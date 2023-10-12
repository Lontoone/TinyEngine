#pragma once
#ifndef UI_PANEL_H
#define UI_PANEL_H
#include "UiItem.h"
//#include <iostream>
#include <vector>

using namespace std;

class UiPanel
{
public:
	//UiPanel();
	//~UiPanel();
	virtual void draw();
	void add_item(UiItem * item);

	vector<UiItem*> m_items;

private:

};

#endif // !UI_PANEL_H
