#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <iostream>
#include <BasicFlags.h>
#include <vector>
#include <string>
#include <map>
//#include "camera->h"
//#include "Component.h"
#include "GameObject.h"
class GameObject;
//#include "TransformObject.h"
#include "UiPanel.h"

using namespace std;


class Hierarchy : public UiPanel
{
public:
	static Hierarchy& instance() {
		return sInstance;
	}

	void execute(EXECUTE_TIMING timimg);
	void draw_ui_loop();	

	void add_object(GameObject* new_obj);
	void add_indirect_object(GameObject* indirec_groups);
	void add_main_camera();
	vector<GameObject*> m_game_objects;

	vector<string>* get_gameobjs_list();
	GameObject* get_gameobjs_by_name(string name);

private:	
	//vector<ExecuteList*> m_execute_layers;
	map<string, GameObject*> m_gameobj_maps;
	vector<string> m_gameobject_names;

	Hierarchy() {};
	static Hierarchy sInstance ;
	
};

#endif 
