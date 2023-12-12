#include "Hierarchy.h"

void Hierarchy::execute(EXECUTE_TIMING timimg ) {
	// Execute self
	if (timimg == EXECUTE_TIMING::BEFORE_FRAME) {
		this->do_before_frame();
	}
	else if(timimg == EXECUTE_TIMING::AFTER_FRAME)
	{
		this->do_after_frame();
	}
	else if (timimg == EXECUTE_TIMING::MAIN_LOGIC)
	{
		this->do_frame();
	}

	// Execute Others 
	for (int i = 0; i < sInstance.m_game_objects.size(); i++) {		
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
void Hierarchy::add_light(Light* light)
{
	this->m_ligths.push_back(light);
}

void Hierarchy::add_renderer(Mesh* mesh)
{
	this->m_meshes.push_back(mesh);
}

void Hierarchy::set_main_camera(Camera* camera)
{
	this->m_main_camera = camera;
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

void Hierarchy::update_shadow_maps()
{
}


//===============================
//   Do Not Refference Other Scripts...
//===============================
void Hierarchy::do_before_frame()
{
	
}

void Hierarchy::do_after_frame()
{
}

void Hierarchy::do_frame()
{
}

/*
void Hierarchy::update_shadow_maps()
{
	for (int i = 0; i < this->m_ligths.size(); i++) {
		this->m_ligths[i];
	}
}
*/
