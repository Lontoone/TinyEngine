#pragma once


#ifndef COMPONENT
#define COMPONENT

#include <iostream>
//class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	int m_execute_order = 3;
	const char* m_name = "";
	Component* m_gameobject;
	virtual void Do(){};

	void set_parent(Component* ptr);

	template<class T>
	T* cast_component() {
		//return &(T)*this;
		//return static_cast<T*>(this);
		return (T*)this;
	}
	
};

#endif // !COMPONENT
