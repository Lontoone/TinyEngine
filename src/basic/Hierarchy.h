#pragma once
#ifndef HIERARCHY
#define HIERARCHY

#include "camera.h"
#include "GameObject.h"
#include "Component.h"
#include <iostream>
#include <vector>
#include "TransformObject.h"

using namespace std;


class Hierarchy
{
public :
	
	void execute();	
	void add_object(GameObject* new_obj);
	GameObject* add_new_object();
	void add_main_camera();

private:
	vector<GameObject*> m_game_objects;
	//vector<ExecuteList*> m_execute_layers;

};

#endif 
