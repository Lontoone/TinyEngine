#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
class TransformObject {
public:
	vec3 m_position = vec3(0.0f);
	vec3 m_scale = vec3(1.0f);
	vec3 m_rotation = vec3(0.0f);

	mat4 m_mvp;

private:
	void UpdateTransform();
};