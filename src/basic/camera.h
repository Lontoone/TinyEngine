#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
	NONE = 0,
	FORWARD , 
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public :
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;

	glm::vec3 worldUp;

	float yaw;  //x-axis
	float pitch;
	float speed;
	float zoom;

	Camera(glm::vec3  position);
	void updateCameraDirection(double dx, double dy);
	void updateCameraPos(CameraDirection dir , double dt);
	void updateCameraZoom(double dy);

	glm::mat4 getViewMatrix();

private :
	void updateCameraVectors();



};

#endif // !CAMERA_H


