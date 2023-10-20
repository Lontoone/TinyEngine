#include "UiItem.h"

void UiItem::add_draw_item(t_draw_cmd& _draw_cmd)
{
	this->m_draw_cmds.push_back(_draw_cmd);
}

void UiItem::run_all_draw_cmd()
{
	for (auto cmd : this->m_draw_cmds) {
		cmd();
	}
}
