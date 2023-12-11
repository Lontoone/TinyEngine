#include "Light.h"

Light::Light()
{
	this->init_shader();
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

void Light::bind_shadow_map()
{
	this->fbo->activate();
	mat4 proj_matrix = this-> get_projection_matrix();
	//mat4 model_matrix = this->get_gameobject()->m_transform->m_model_matrix;
	mat4 light_view_matrix = this->get_gameobject()->m_transform->m_translate_matrix * mat4(-1) ;// inverse translation 
	this->m_vp_matrix= proj_matrix * light_view_matrix;

	this->shadow_shader.activate();
	const char* vp_name = u_LIGHT_VP_MATRIX;
	glUniformMatrix4fv(this->shadow_shader.shader_variables[vp_name] ,1 ,GL_FALSE, value_ptr(this->m_vp_matrix) );


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
	// Bind shadow fbo
	this->bind_shadow_map();	
}

void Light::light_pass()
{
}

void Light::init_shader()
{
	this->shadow_shader = Shader(
		GET_SRC_FOLDER() + string( "\\assets\\shaders\\shadow_map_vert.glsl"),
		GET_SRC_FOLDER() + string( "\\assets\\shaders\\shadow_map_frag.glsl")
	);
	const char* uniform_name = u_LIGHT_VP_MATRIX;
	this->shadow_shader.add_variables(uniform_name);
}

mat4 Light::get_directional_light_mvp()
{
	// return otho proj matrix
	Camera* main_camera = Hierarchy::instance().get_main_camera();
	float near_plane = main_camera->m_near;
	float far_plane = main_camera->m_far;
	return glm::ortho(0.0f , main_camera->m_width , main_camera->m_heigth ,0.0f , main_camera->m_near , main_camera->m_far );
}
/*
*/
