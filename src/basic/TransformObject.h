#pragma once
#ifndef TRANSFORM_OBJECT
#define TRANSFORM_OBJECT

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef COMPONENT
#include "Component.h"
#endif // !COMPONENT


using namespace glm;
using namespace std;
class TransformObject : virtual public Component {
public:
	~TransformObject() =default;
	TransformObject();
	vec3 m_position = vec3(0.0f);
	vec3 m_scale = vec3(1.0f);
	vec3 m_rotation = vec3(0.0f);
	vec3 m_forward = vec3(0, 0, 1);
	vec3 m_up = vec3(0, 1, 0);
	vec3 m_right = vec3(1, 0, 0);

	mat4 m_rot_matrix;
	mat4 m_scale_matrix = glm::mat4(1.0f);
	mat4 m_translate_matrix;
	mat4 m_model_matrix = glm::mat4(1.0f);

	void move(vec3 _m);
	void Do() override;

private:
	//void UpdateTransform();
	void update_rotation_matrix_eular();
	void update_translate_matrix();
	void update_scale_matrix();

};

#endif // TRANSFORM_OBJECT