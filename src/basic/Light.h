#pragma once
#include <glm/glm.hpp>
/*
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/
#include <Component.h>

/*
*/
#ifndef GAMEOBJECT
#include <GameObject.h>
#endif // !GAMEOBJECT

//class GameObject;
#include <BasicFlags.h>
#include <FrameBufferObject.h>
using namespace glm;
using namespace std;


class Light:public Component
{
public:
	Light();
	~Light();
	static const unsigned int SHOWOW_CASCADE_LEVEL = 1;
	unsigned int SHOWOW_RESOLUTION = 1024;
	LIGHT_Type light_type;	
	void Do() override {};
	void update_shadow_map();
	mat4 get_projection_matrix();


private:
	void init_buffer();
	void shadow_pass();
	void light_pass();
	FramebufferObject* fbo; 
	mat4 get_directional_light_mvp();
	unsigned int shadow_fbo_cascaded[SHOWOW_CASCADE_LEVEL];
	Shader shadow_shader;

};


