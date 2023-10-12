#pragma once
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <iostream>
#include <vector>
#include "TransformObject.h"
#include "Mesh.h"
//#include "Hierarchy.h"

#ifndef COMPONENT
#include "Component.h"
#endif // !COMPONENT

/*

struct ExecuteList
{
	vector<Component*> m_comps;
};
*/


using namespace std;

class GameObject : public Component
{
public:
	GameObject();
	~GameObject();

	const char* name;
	TransformObject					*m_transform;	
	std::vector<Component*>			m_comps;
	void execute();

	void set_name(const char* new_name);
	void add_component(Component* _new_comp );

	template<class T>
	T* get_component() {
		//return &(T)*this;
		//return static_cast<T*>(this);
		std::cout << "before cast " << this << std::endl;
		return (T*)this;
	}
private:
	//std::vector<ExecuteList*> m_execute_layers;

};

#endif 