#include "Component.h"
Component::Component()
{
	this->m_execute_order = 3;
	this->m_name = "New_Comp";
}
Component::~Component()
{
	std::cout << "deconstruct comp" << this << std::endl;
}

/*
GameObject* Component::get_parent()
{
	return this->m_parent;
}

void Component::set_parent(GameObject* _newP)
{
	this->m_parent = _newP;
}
*/

void Component::set_parent(Component* ptr) {
	this->m_gameobject = ptr;
}

/*
template<class T>
T* Component::get_component()
{
	//return *<T>(this);
	return dynamic_cast<T>(this);
	//return nullptr;
}
*/