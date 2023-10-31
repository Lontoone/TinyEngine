#include "camera.h"
Camera::Camera() :GameObject("Camera")
{
}

Camera::Camera(glm::vec3 position):GameObject("Camera") {
	this->m_transform->m_position = position;
	//this->m_transform->m_auto_update_matrix = false;
}
/*
Camera::Camera(glm::vec3 position) 
	:	cameraPos(position), 
		worldUp(glm::vec3(0.0f , 1.0f , 0.0f)),
		yaw(-90.0f) ,
		pitch(0.0f),
		speed(2.5f),
		zoom(45.0f),
		cameraFront(glm::vec3 (0.0f,0.0f,-1.0f)) // Look at origin
{
	updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx;
	pitch += dy;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
}
void Camera::updateCameraPos(CameraDirection dir, double dt) {
	float velocity = (float)dt * speed; 

	switch (dir)
	{
	case CameraDirection::FORWARD:
		cameraPos += cameraFront * velocity;
		break;
	case CameraDirection::BACKWARD:
		cameraPos -= cameraFront * velocity;
		break;
	case CameraDirection::RIGHT:
		cameraPos += cameraRight * velocity;
		break;
	case CameraDirection::LEFT:
		cameraPos -= cameraRight * velocity;
		break;
	case CameraDirection::UP:
		cameraPos += cameraUp * velocity;
		break;
	case CameraDirection::DOWN:
		cameraPos -= cameraUp * velocity;
		break;

	default:
		break;
	}

}
*/
void Camera::updateCameraZoom(double dy) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= dy;
	}
	else if (zoom < 1.0f) {
		zoom = 1.0f;
	}
	else {
		zoom = 45.0f;
	}
}

vec3 Camera::get_view_center_position()
{
	vec3 center = this->m_transform->m_position + this->m_transform->m_forward * zoom;
	return center;
}

vec3 Camera::get_view_dir()
{
	//vec3 dir =this->m_transform->m_position 
	//return -this->m_transform->m_forward;
	//return normalize(this->m_transform->m_position - this->get_view_center_position());
	return  normalize(this->view_target) -normalize(this->m_transform->m_position);
}

glm::mat4 Camera::getViewMatrix() {
	//this->updateCameraVectors();	
	//return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);		
	//vec3 _tmp = this->get_view_center_position();
	//vec3 _tmp = this->m_transform->m_forward;
	//cout << _tmp.x << " , " << _tmp.y << "," << _tmp.z << endl;
	// 
	//return glm::lookAt( this->m_transform->m_position , this->get_view_dir() , this->m_transform->m_up);
	
	//mat4 look = glm::lookAt(this->m_transform->m_position , this->get_view_dir(), this->m_transform->m_up);
	//this->m_transform->m_model_matrix *= look;
	return inverse( this->m_transform->m_model_matrix);
}

/*
void Camera::updateCameraVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp	= glm::normalize(glm::cross(cameraRight, cameraFront));

}
*/