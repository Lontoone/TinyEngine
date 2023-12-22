#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "GameObject.h"
#include <UniformBinder.h>

enum class CameraDirection {
	NONE = 0,
	FORWARD , 
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

using namespace glm;
class Camera : public Component , public UniformBinder {
public :
	
	Camera();	
	Camera(glm::vec3  position , float ratio ,float near, float far);
	Camera(glm::vec3  position, float near, float far , float width , float height);
	
	
	float speed;
	float m_aspect_ratio;
	float m_near = 0.1f;
	float m_far = 10.0f;
	float m_width = 0;  //ToDo:....
	float m_heigth = 0;

	vec3 view_target = vec3(0.0f);
	float zoom = 3;	

	void Do() override;	
	void bind_uniform(unsigned int shader_pid) override;
	void viewFrustumClipPlaneCornersInViewSpace(const float depth, float* corners);
	mat4 get_model_matrix() { return this->get_gameobject()->m_transform->m_model_matrix; };
	void resize(unsigned int width , unsigned int height);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getInverseProjectionMatrix();

private :
	//void updateCameraVectors();

};

#endif // !CAMERA_H



