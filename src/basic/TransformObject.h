#pragma once
#ifndef TRANSFORM_OBJECT
#define TRANSFORM_OBJECT

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Hierarchy.h>
#include <functional>
#include <UIableComponent.h>
#include <GameObject.h>
class GameObject;
//#include "Component.h"
//#include "UiPanel.h"

using namespace glm;
using namespace std;

const vec3 WORLD_UP			= vec3(0 , 1 ,0);
const vec3 WORLD_FORWARD	= vec3(0 , 0, 1);
const vec3 WORLD_RIGHT		= vec3(1 , 0, 0);

//class TransformObject : virtual public Component ,virtual public UiPanel{
class TransformObject :public UiableComponent {
public:
	~TransformObject() =default;
	TransformObject();
	TransformObject(GameObject* _obj);
	bool m_auto_update_matrix = true;
	vec3 m_position = vec3(0.0f);
	vec3 m_world_position = vec3(0.0f);
	vec3 m_scale = vec3(1.0f);
	vec3 m_rotation = vec3(0.0f);
	vec3 m_forward = vec3(0, 0, 1);
	vec3 m_up = vec3(0, 1, 0);
	vec3 m_right = vec3(1, 0, 0);

	mat4 m_rot_matrix;
	mat4 m_scale_matrix = glm::mat4(1.0f);
	mat4 m_translate_matrix;
	mat4 m_model_matrix = glm::mat4(1.0f);

	TransformObject* m_parent=nullptr; 
	GameObject* m_gameobject;

	void move(vec3 _m);
	void set_transform_parent(TransformObject* _new_parent);

	// Overide for Ui
	void Do() override;
	void init_ui_content() override;
	Component* copy() override;

	void update_rotation_matrix_eular();
	void update_translate_matrix();
	void update_scale_matrix();
	void update_local();
protected:
	//void UpdateTransform();

};

#endif // TRANSFORM_OBJECT