#include "camera.h"


Camera::Camera(glm::vec3 position, float ratio, float near, float far):TransformObject(){
	this->m_parent = new TransformObject();

	this->m_position = position;
	this->m_auto_update_matrix = false;

	this->update_translate_matrix();
	this->update_rotation_matrix_eular();  // TODO: qutanion
	this->update_scale_matrix();

	this->m_forward = this->get_view_dir();
	this->m_right = glm::normalize(glm::cross(this->m_forward, WORLD_UP));
	this->m_up = glm::normalize(glm::cross(this->m_right, this->m_forward));

	this->m_aspect_ratio = ratio;
	this->m_far = far;
	this->m_near = near;
	//delete this->m_gameobject->m_transform;
	//this->m_gameobject->m_transform = this;
}
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
	vec3 center = this->m_position + this->m_forward * zoom;
	return center;
}

vec3 Camera::get_view_dir()
{
	//vec3 dir =this->m_transform->m_position 
	//return -this->m_transform->m_forward;
	//return normalize(this->m_transform->m_position - this->get_view_center_position());
	return  normalize(this->m_position - this->view_target);
}

void Camera::Do()
{	
	//this->m_parent->Do();
	//this->m_position = -this->m_forward * zoom + view_offset;
	//this->m_translate_matrix = glm::translate(mat4(1.0f), this->m_position + this->view_target);
	
	this->update_rotation_matrix_eular();  // TODO: qutanion
	this->update_translate_matrix();
	this->update_scale_matrix();
	
	/*
	if (this->m_parent != nullptr)
		this->m_model_matrix = this->m_parent->m_model_matrix * (this->m_translate_matrix) * this->m_rot_matrix;
	else
		this->m_model_matrix = this->m_translate_matrix * this->m_rot_matrix * this->m_scale_matrix;
	*/
	this->m_model_matrix = this->m_translate_matrix * this->m_rot_matrix * this->m_scale_matrix;
	this->update_local();

}

void Camera::draw_frustum()
{
}

void Camera::viewFrustumClipPlaneCornersInViewSpace(const float depth, float* corners)
{
	// Get Depth in NDC, the depth in viewSpace is negative
	const glm::vec4 v = glm::vec4(0, 0, -1 * depth*0.5, 1);   //[TODO :TEMP ] 不知道為甚麼都差2倍
	glm::vec4 vInNDC = this->getProjectionMatrix() * v;
	vInNDC.z = vInNDC.z / vInNDC.w;

	// Get 4 corner of the clip plane
	float cornerXY[] = {
		-1, 1,
		-1, -1,
		1, -1,
		1, 1
	};
	for (int j = 0; j < 4; j++) {
		glm::vec4 wcc = {
			cornerXY[j * 2 + 0], cornerXY[j * 2 + 1], vInNDC.z, 1
		};
		mat4 invproj_mat = this->getInverseProjectionMatrix();
		wcc = invproj_mat * wcc;
		wcc = wcc / wcc.w;

		corners[j * 3 + 0] = wcc[0];
		corners[j * 3 + 1] = wcc[1];
		corners[j * 3 + 2] = wcc[2];
	}
}

glm::mat4 Camera::getViewMatrix() {	

	this->Do();
	//return lookAt(this->m_position, this->m_position +this->m_forward * this->zoom , this->m_up);
	return inverse( this->m_model_matrix) ;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return  glm::perspective(glm::radians(45.0f), this->m_aspect_ratio, this->m_near, this->m_far );	
	/*
	float left = 0.0f;
	float right = static_cast<float>(1500);
	float bottom = 0.0f;
	float top = static_cast<float>(720);
	float near = -1.0f;
	float far = 1.0f;

	return glm::ortho(left, right, bottom, top, near, far);
	*/
}

glm::mat4 Camera::getInverseProjectionMatrix()
{
	return inverse(this->getProjectionMatrix());
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