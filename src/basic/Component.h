#pragma once


#ifndef COMPONENT
#define COMPONENT

#include <iostream>
//class GameObject;

class Component
{
public:
	Component();
	Component(const char* cmp_name);
	virtual ~Component();

	int m_execute_order = 3;
	const char* m_name = "";
	uint32_t  m_comp_id;
	

	Component* m_gameobject;
	virtual void Do(){};
	virtual void DO_Before_Frame() {};
	virtual void Do_End_Frame() {};

	void set_parent(Component* ptr);

	template<class T>
	T* cast_component() {
		//return &(T)*this;
		//return static_cast<T*>(this);
		return (T*)this;
	}

	
};

#endif // !COMPONENT
