#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "GameObject.h"

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
class Camera : public TransformObject {
public :
	Camera();	
	Camera(glm::vec3  position , float ratio ,float near, float far);
	
	
	float yaw =-90.0f;  //x-axis
	float pitch =0.0f;
	float speed;
	float m_aspect_ratio;
	float m_near = 0.1f;
	float m_far = 10.0f;

	vec3 view_target = vec3(0.0f);
	float zoom = -3;
	void updateCameraDirection(double dx, double dy);
	//void updateCameraPos(CameraDirection dir , double dt);
	void updateCameraZoom(double dy);
	

	vec3 get_view_center_position();
	vec3 get_view_dir();

	void Do() override;	
	void draw_frustum();
	void viewFrustumClipPlaneCornersInViewSpace(const float depth, float* corners);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private :
	//void updateCameraVectors();



};

#endif // !CAMERA_H



