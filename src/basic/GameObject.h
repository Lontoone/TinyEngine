#pragma once
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <BasicFlags.h>
#include <iostream>
#include <vector>
//#include "TransformObject.h"
#include <TransformObject.h>
class TransformObject;
#include <string>
#include <random>
#include <UiPanel.h>
#include <UIableComponent.h>
#include <algorithm>
//#include "../utlis/uuid_helper.h"
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

class GameObject : public Component , public UiPanel
{
public:
	GameObject();
	GameObject(TransformObject *trans);
	GameObject(const char* _obj_name);
	~GameObject();
		
	//const char* name ="";
	string name="";
	const char* m_instance_id="";
	//int32 m_instance_id =0;

	std::vector<Component*>			m_comps;
	std::vector<GameObject*>		m_childs;
	
	TransformObject*				m_transform;	

	void execute(EXECUTE_TIMING timimg);
	void update_ui_tree();

	void set_name(const char* new_name);	
	void add_component(Component* _new_comp );
	void add_component(UiableComponent* _new_comp);	
	
	void DO_Before_Frame() override;
	void Do_End_Frame() override;

	bool add_child(GameObject* _child);
	void remove_child(GameObject* _child);
	bool check_is_child(GameObject* _child);
	//Component* copy()override;
	vector<GameObject*> clone();

	template<class T>
	T* get_component() {
		//return &(T)*this;
		//return static_cast<T*>(this);
		std::cout << "before cast " << this << std::endl;
		return (T*)this;
	}
private:
	//std::vector<ExecuteList*> m_execute_layers;
	std::vector<UiPanel*>			m_components_panels;

};

#endif 