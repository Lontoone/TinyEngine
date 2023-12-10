#include "Light.h"

Light::Light()
{
	this->init_buffer();
}

Light::~Light()
{
	delete this->fbo;
}

void Light::update_shadow_map()
{
	for (int i = 0; i < this->SHOWOW_CASCADE_LEVEL; i++) {
		this->fbo->activate();
		//mat4 proj_matrix = this-> get_projection_matrix();
		//mat4 model_matrix = this->get_gameobject()->m_transform->m_model_matrix;
		//mat4 WVP =  ;
	}
}
/*
mat4 Light::get_projection_matrix()
{
	if (this->light_type == LIGHT_Type::DIRECTIONAL) {		
		return this->get_directional_light_mvp();
	}
	//TODO:

	return mat4();
}
*/

void Light::init_buffer()
{	
	this->shadow_shader = Shader(); //TODO....
	/*
	this->fbo = new FramebufferObject(
			&this->shadow_shader ,
			&COLOR_FRAME_BUFFERS[0],
			1 ,
			SHOWOW_RESOLUTION , SHOWOW_RESOLUTION);
	*/
	for (int i = 0; i < this->SHOWOW_CASCADE_LEVEL; i++) {



	}
}

/*
mat4 Light::get_directional_light_mvp()
{
	// return otho proj matrix
	return mat4();
}
*/
