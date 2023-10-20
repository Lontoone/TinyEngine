#include "Hierarchy.h"

/*
void Hierarchy::execute()
{
	for (int i = 0; i < this->m_execute_layers.size; i++) {		
		auto _comps = &this->m_execute_layers[i]->m_comps;
		for (int j = 0; j < _comps->size; j++) {
			_comps[j].Do();
		}
	
	}
}

*/
void Hierarchy::execute(EXECUTE_TIMING timimg ) {
	for (int i = 0; i < this->m_game_objects.size(); i++) {		
		this->m_game_objects[i]->execute(timimg);
	}
}
void Hierarchy::draw_ui_loop()
{
	for (int i = 0; i < this->m_game_objects.size(); i++) {
		this->m_game_objects[i]->update_ui_tree();
	}
}
/*
void Hierarchy::execute_before_frame()
{
	for (int i = 0; i < this->m_game_objects.size(); i++) {
		this->m_game_objects[i]->execute();
	}
}

void Hierarchy::execute_after_frame()
{

}
*/

void Hierarchy::add_object(GameObject* new_obj)
{
	cout << "add cmp " << new_obj->name << endl;
	this->m_game_objects.push_back(new_obj);

	this->m_gameobject_names.push_back(new_obj->name);
	this->m_gameobj_maps[new_obj->name ]= new_obj;
	//this->add_ui_items(new_obj);
}
/*
void Hierarchy::add_ui_items(Component* new_cmp)
{
	UiItem* ui_item = dynamic_cast<UiItem*>(new_cmp);
	if (ui_item != NULL) {
		//ui_item->run_all_draw_cmd();
		this->m_items.push_back(ui_item);
	}
}
*/
/*
GameObject* Hierarchy::add_new_object()
{
	return nullptr;
}
*/
void Hierarchy::add_main_camera()
{
	cout << "add main camera" << endl;
	//auto gobj = this->add_new_object();


}

vector<string>* Hierarchy::get_gameobjs_list()
{
	return &this->m_gameobject_names;
}

GameObject* Hierarchy::get_gameobjs_by_name(string name)
{	
	return this->m_gameobj_maps[name];
}
