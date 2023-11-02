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

class Camera : public TransformObject {
public :
	Camera();	
	~Camera();
	/*
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;

	glm::vec3 worldUp;

	glm::vec3 camera_center;

	*/
	float yaw =-90.0f;  //x-axis
	float pitch =0.0f;
	float speed;

	vec3 view_target = vec3(0.0f);
	float zoom = -3;
	Camera(glm::vec3  position);
	void updateCameraDirection(double dx, double dy);
	//void updateCameraPos(CameraDirection dir , double dt);
	void updateCameraZoom(double dy);
	

	vec3 get_view_center_position();
	vec3 get_view_dir();

	void Do() override;

	glm::mat4 getViewMatrix();

private :
	//void updateCameraVectors();



};

#endif // !CAMERA_H


