#include "Hierarchy.h"

void Hierarchy::execute(EXECUTE_TIMING timimg ) {
	for (int i = 0; i < sInstance.m_game_objects.size(); i++) {
		//this->m_game_objects[i]->execute(timimg);
		sInstance.m_game_objects[i]->execute(timimg);
	}
}
void Hierarchy::draw_ui_loop()
{
	for (int i = 0; i < sInstance.m_game_objects.size(); i++) {
		sInstance.m_game_objects[i]->update_ui_tree();
	}
}
void Hierarchy::add_object(GameObject* new_obj)
{
	cout << "add cmp " << new_obj->name << endl;
	sInstance.m_game_objects.push_back(new_obj);

	sInstance.m_gameobject_names.push_back(new_obj->name);
	sInstance.m_gameobj_maps[new_obj->name ]= new_obj;
	//this->add_ui_items(new_obj);
}
//void Hierarchy::add_object(IndirectInstancedMesh* indirect_groups){}
void Hierarchy::add_main_camera()
{
	cout << "add main camera" << endl;
	//auto gobj = this->add_new_object();

}

vector<string>* Hierarchy::get_gameobjs_list()
{
	return &sInstance.m_gameobject_names;
}

GameObject* Hierarchy::get_gameobjs_by_name(string name)
{	
	return sInstance.m_gameobj_maps[name];
}
