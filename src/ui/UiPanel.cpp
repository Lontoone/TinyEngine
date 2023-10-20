#include "UiPanel.h"


//void UiPanel::add_draw_item(t_draw_cmd& _draw_cmd)
void UiPanel::add_draw_item(function<bool()> _draw_cmd)
{
	this->m_draw_cmds.push_back(_draw_cmd);
}

void UiPanel::add_drop_down_menu(vector<string>& _ops, function < bool (string) > _callback)
{
	string current_item = "";
	
	if (ImGui::BeginCombo("##combo", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
	{
		//for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		//for (int n = 0; n < _ops->size(); n++)
		for (auto opt : _ops)
		{
			bool is_selected = (current_item == opt ); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(opt.c_str(), is_selected)) {
				current_item = opt;
				std::cout << "selected " << current_item << std::endl;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();

	}
}

void UiPanel::draw_ui_panel()
{

	//Begin(this->m_panel_names); 
	//this->panel_begin();
	//Begin("Hi");
	for (auto _cmd : this->m_draw_cmds) {
		//_item->run_all_draw_cmd();
		_cmd();
	}

	//End();
	//this->panel_end();
}


void UiPanel::create_panel(const char* window_name)
{
	this->m_panel_names = window_name;
}

void UiPanel::panel_begin()
{
	if (this->m_panel_names == "") {
		this->m_panel_names = "No Name";
	}
	SetNextWindowPos(GetMainViewport()->Pos);
	Begin(this->m_panel_names);
}

void UiPanel::panel_end()
{
	End();
}


