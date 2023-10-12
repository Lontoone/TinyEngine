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
void Hierarchy::execute() {
	for (int i = 0; i < this->m_game_objects.size(); i++) {
		this->m_game_objects[i]->execute(); 
	}
}
void Hierarchy::add_object(GameObject* new_obj)
{
	cout << "add cmp " << new_obj->name << endl;
	this->m_game_objects.push_back(new_obj);
}
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
