#include "Light.h"

Light::Light()
{
	this->init_buffer();
}

Light::~Light()
{
	delete this->fbo;
}

void Light::Do()
{
	//Bind shadow fbo
	this->shadow_pass();

	//
	this->light_pass();
}

void Light::update_shadow_map()
{
	this->fbo->activate();
	mat4 proj_matrix = this-> get_projection_matrix();
	mat4 model_matrix = this->get_gameobject()->m_transform->m_model_matrix;
	mat4 mvp = proj_matrix * model_matrix;
	// ToDo: Cascade shadow...
	//for (int i = 0; i < this->SHOWOW_CASCADE_LEVEL; i++) {}
}

mat4 Light::get_projection_matrix()
{
	if (this->light_type == LIGHT_Type::DIRECTIONAL) {		
		return this->get_directional_light_mvp();
	}
	//TODO:

	return mat4();
}
/*
*/

void Light::init_buffer()
{	
	this->shadow_shader = Shader(); //TODO....
	
	this->fbo = new FramebufferObject(
			&this->shadow_shader ,
			GL_DEPTH_ATTACHMENT,
			GL_DEPTH_COMPONENT,
			GL_NONE,
			GL_NONE,
			SHOWOW_RESOLUTION , SHOWOW_RESOLUTION);
	// ToDo: cascade shadow
	//for (int i = 0; i < this->SHOWOW_CASCADE_LEVEL; i++) {	}

}

void Light::shadow_pass()
{
	this->update_shadow_map();
	// Bind shadow fbo
	//this->fbo->activate();
	// Set light mvp

	// Draw the scene 
}

void Light::light_pass()
{
}

mat4 Light::get_directional_light_mvp()
{
	// return otho proj matrix
	Camera* main_camera = Hierarchy::instance().main_camera;
	float near_plane = main_camera->m_near;
	float far_plane = main_camera->m_far;
	return glm::ortho(0.0f , main_camera->m_width , main_camera->m_heigth ,0.0f , main_camera->m_near , main_camera->m_far );
}
/*
*/
