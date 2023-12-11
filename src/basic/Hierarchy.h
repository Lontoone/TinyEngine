#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <iostream>
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
	//void add_light(Light* light);
	void add_indirect_object(GameObject* indirec_groups);
	void add_main_camera();

	vector<string>* get_gameobjs_list();
	GameObject* get_gameobjs_by_name(string name);

	vector<GameObject*> m_game_objects;
	Camera* main_camera;
	//float get_main_camera_near() { return main_camera->m_near; };

private:	
	//vector<ExecuteList*> m_execute_layers;
	map<string, GameObject*> m_gameobj_maps;
	vector<string> m_gameobject_names;
	vector<Light*> m_ligths;
	Hierarchy() {};
	static Hierarchy sInstance ;
	
	void update_shadow_maps();
};

#endif 
