#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/
#include <ltc_matrix.hpp>
#include <UiableComponent.h>
#include <env_path.h>
/*
*/
#include <Camera.h>
#ifndef GAMEOBJECT
#include <GameObject.h>
#endif // !GAMEOBJECT

//class GameObject;
#include <BasicFlags.h>
#include <FrameBufferObject.h>
using namespace glm;
using namespace std;


class Light : public UiableComponent
{
public:
	Light();
	Light(LIGHT_Type type);
	~Light();
	static const unsigned int SHOWOW_CASCADE_LEVEL = 1;
	unsigned int SHOWOW_RESOLUTION = 1024;
	LIGHT_Type light_type = LIGHT_Type::DIRECTIONAL;	
	void Do() override;
	mat4 get_projection_matrix();
	mat4 get_light_view_matrix();
	mat4 get_light_vp_matrix();
	mat4 m_vp_matrix;
	void shadow_pass();
	void light_pass();

	FramebufferObject* fbo; 
	Shader shadow_shader;
	float m_far=10;

	// point light
	mat4 m_point_light_view_matrixs [6];
	mat4 m_point_light_vp_matrixs[6];

	// area light

	void init_ui_content() override;

private:
	void bind_shadow_map();
	void init_buffer();
	void init_shader();
	
	unsigned int shadow_fbo_cascaded[SHOWOW_CASCADE_LEVEL];

};


