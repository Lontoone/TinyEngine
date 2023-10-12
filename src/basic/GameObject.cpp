#include "GameObject.h"

GameObject::GameObject()
{
	TransformObject* trans = new TransformObject();	
	this->add_component(trans);
	this->m_transform = trans;
	/*
	*/
}

GameObject::~GameObject()
{
	for (auto ptr : this->m_comps) {
		cout << "freeing " << ptr << endl;
		delete ptr;
	}
	/*
	
	for (int i = 0; i < this->m_comps.size(); i++) {
		cout << "freeing " << m_comps[i] << endl;
		delete this->m_comps[i];
	}*/

	this->m_comps.clear();
	cout << this->m_comps.size()<<endl;
}

void GameObject::execute()
{	
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
	this->m_comps.push_back(_new_comp);
	_new_comp->set_parent(this);

}
