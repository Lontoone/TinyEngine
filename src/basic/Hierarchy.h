#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H
#include <iostream>
#include <type_traits>
#include <BasicFlags.h>
#include <vector>
#include <string>
#include <map>
//#include "camera->h"
#include "Component.h"
#include "GameObject.h"
//#include <camera.h>
#include "UiPanel.h"
//#include <Light.h>
//class GameObject;
//#include "TransformObject.h"

//================================
//    Forward Declearation
// Use forward declearation as more as posiable to avoid include error
//================================
class Light;
class Camera;

using namespace std;


class Hierarchy : public UiPanel
{
public:
	static Hierarchy& instance() {
		return sInstance;
	}

	void execute(EXECUTE_TIMING timimg);
	void draw_ui_loop();	

	void set_main_camera(Camera* camera);
	void add_object(GameObject* new_obj);
	void add_light(Light* light);
	//void add_indirect_object(GameObject* indirec_groups); // ToDo...
	void add_main_camera();

	vector<string>* get_gameobjs_list();
	GameObject* get_gameobjs_by_name(string name);

	vector<GameObject*> m_game_objects;
	Camera* get_main_camera() { return this->m_main_camera; }
	//float get_main_camera_near() { return main_camera->m_near; };

	vector<Light*> m_ligths;

private:	
	static Hierarchy sInstance ;
	//vector<ExecuteList*> m_execute_layers;
	Camera* m_main_camera;
	map<string, GameObject*> m_gameobj_maps;
	vector<string> m_gameobject_names;
	Hierarchy() {};
	void update_shadow_maps();
	
	
};

#endif 
