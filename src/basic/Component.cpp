#include "Component.h"
Component::Component()
{
	this->m_comp_id = reinterpret_cast<uint32_t>(this);
	this->m_execute_order = 3;
	//this->m_name = "New_Comp";

}
Component::Component(const char* cmp_name):Component()
{
	this->m_name = cmp_name;
}
Component::~Component()
{
	std::cout << "deconstruct comp" << this << std::endl;
}

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