#include "GameObject.h"

void GameObject::execute()
{
	/*
	for (int i = 0; i < this->m_execute_layers.size; i++) {

		auto _comps = &this->m_execute_layers[i]->m_comps;
		for (int j = 0; j < _comps->size; j++) {
			
		}

	}
	*/
	for (int i = 0; i < this->m_comps.size(); i++) {
		this->m_comps[i]->Do();
	}
}

void GameObject::set_name(const char* new_name)
{
	this->name = new_name;
}

void GameObject::add_component(Component* _new_comp)
{
	cout << "add new comp" << _new_comp->m_name << endl;
	this->m_comps.push_back(_new_comp);

}
