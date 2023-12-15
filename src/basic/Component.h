#pragma once


#ifndef COMPONENT
#define COMPONENT

#include <iostream>

//#include <GameObject.h>
class GameObject;
class Shader;

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
	GameObject* get_gameobject() { return (GameObject*)this->m_gameobject; };
		
	virtual Component* copy() { return nullptr; };
	virtual void bind_uniform(unsigned int shader_pid) {};

	void attatch_to(Component* ptr);

	template<class T>
	T* cast_component() {
		//return &(T)*this;
		//return static_cast<T*>(this);
		return (T*)this;
	}
	/*
	template<class T>
	T* try_cast_component() {
		return dynamic_cast<T*>(this);
	}
	*/

	
};

#endif // !COMPONENT
