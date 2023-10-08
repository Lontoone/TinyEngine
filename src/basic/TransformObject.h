#pragma once
#ifndef TRANSFORM_OBJECT
#define TRANSFORM_OBJECT

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

using namespace glm;
using namespace std;
class TransformObject :public Component {
public:
	vec3 m_position = vec3(0.0f);
	vec3 m_scale = vec3(1.0f);
	vec3 m_rotation = vec3(0.0f);

	mat4 m_mvp;

private:
	//void UpdateTransform();
	void Do() override;
};

#endif // TRANSFORM_OBJECT