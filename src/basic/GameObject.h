#pragma once
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <iostream>
#include <vector>
#include "TransformObject.h"
#include "Mesh.h"
//#include "Hierarchy.h"
#include "Component.h"
/*

struct ExecuteList
{
	vector<Component*> m_comps;
};
*/


using namespace std;
class GameObject
{
public:
	const char* name;
	TransformObject				m_transform_cmp;
	//Mesh						m_mesh_cmp;
	std::vector<Component*>			m_comps;
	void execute();

	void set_name(const char* new_name);
	void add_component(Component* _new_comp );
private:
	//std::vector<ExecuteList*> m_execute_layers;

};

#endif 