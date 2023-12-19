#include "Light.h"

Light::Light()
{	
	this->light_type = LIGHT_Type::DIRECTIONAL;
	this->m_far = 10;
	this->init_shader();
	this->init_buffer();
	init_ui_content();
}

Light::Light(LIGHT_Type type)
{
	this->light_type = type;
	this->m_far = 10;	
	this->init_shader();
	this->init_buffer();
	init_ui_content();
}

Light::~Light()
{	
	delete this->fbo;
}

void Light::Do()
{
	//Bind shadow fbo
	//this->shadow_pass();
	//this->light_pass();
	
}
// Not Using
void Light::bind_shadow_map()
{
	//this->fbo->activate();
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
		Camera* main_camera = Hierarchy::instance().get_main_camera();
		float near_plane = main_camera->m_near;
		float far_plane = main_camera->m_far;

		float width = 5;
		float height = 5;
		//return glm::ortho(0.0f, float(this->SHOWOW_RESOLUTION), float(this->SHOWOW_RESOLUTION), 0.0f, main_camera->m_near, main_camera->m_far);
		//return mat4(1);

		return glm::ortho(-width, width, -height, height, 0.1f, this->m_far);
	}
	else if (this->light_type == LIGHT_Type::POINT_LIGHT) {
		float aspect = (float)this->SHOWOW_RESOLUTION / (float)this->SHOWOW_RESOLUTION;
		float near = 0.22f;
		float far = this->m_far;
		return glm::perspective(glm::radians(90.0f), aspect, near, far);
	}
	//TODO:

	return mat4();
}
mat4 Light::get_light_view_matrix()
{
	if (this->light_type == LIGHT_Type::DIRECTIONAL) {
		// Directional light no translation
		vec3 light_pos = this->get_gameobject()->m_transform->m_position ;
		
		return glm::lookAt(
			light_pos,
			//light_pos + this->get_gameobject()->m_transform->m_forward * 5.0f,
			//vec3(0),
			vec3(0.542, -0.141, -0.422) ,
			WORLD_UP);
		/*
		*/
	}
	else if (this->light_type == LIGHT_Type::POINT_LIGHT) {
		
		vec3 lightPos = this->get_gameobject()->m_transform->m_position;
		float rate = 0.22f;
		this->m_point_light_view_matrixs[0] =(glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0) *rate, glm::vec3(0.0, -1.0, 0.0)));
		this->m_point_light_view_matrixs[1] =(glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0)*rate, glm::vec3(0.0, -1.0, 0.0)));
		this->m_point_light_view_matrixs[2] =(glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0) *rate, glm::vec3(0.0, 0.0, 1.0)));
		this->m_point_light_view_matrixs[3] =(glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0)*rate, glm::vec3(0.0, 0.0, -1.0)));
		this->m_point_light_view_matrixs[4] =(glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0) *rate, glm::vec3(0.0, -1.0, 0.0)));
		this->m_point_light_view_matrixs[5] = glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0)*rate, glm::vec3(0.0, -1.0, 0.0));

		return this->m_point_light_view_matrixs[0];
		/*
		*/
	}
}
mat4 Light::get_light_vp_matrix()
{
	if (this->light_type == LIGHT_Type::DIRECTIONAL) {
		return this->get_projection_matrix() * this->get_light_view_matrix();
	}
	else if (this->light_type == LIGHT_Type::POINT_LIGHT) {		
		mat4 proj = this->get_projection_matrix();
		for (int i = 0; i < 6;i++) {
			this->m_point_light_vp_matrixs[i] = proj * this->m_point_light_view_matrixs[i];
		}
		return this->m_point_light_vp_matrixs[0];
	}
}
/*
*/

void Light::init_buffer()
{		
	if (this->fbo != nullptr) {
		glDeleteFramebuffers(1 , &this->fbo->fbo);		
	}


	if (this->light_type==LIGHT_Type::DIRECTIONAL) {
		this->fbo = new FramebufferObject();
		this->fbo->create_shadow_buffer(
				&this->shadow_shader ,			
				SHOWOW_RESOLUTION , SHOWOW_RESOLUTION);
	}
	else if (this->light_type == LIGHT_Type::POINT_LIGHT) {
		this->fbo = new FramebufferObject();
		this->fbo->create_pointLight_shadow_buffer(
			&this->shadow_shader,
			SHOWOW_RESOLUTION, SHOWOW_RESOLUTION);

	}


	// ToDo: cascade shadow
	//for (int i = 0; i < this->SHOWOW_CASCADE_LEVEL; i++) {	}

}

void Light::init_ui_content()
{
	cout << "Light ui";
	auto title_text = [&]() {Text("========= [ Light ] ============"); return true; };
	this->add_draw_item(title_text);

	// [Active Component ]
	auto activate_checkbox = [&]() {
		ImGui::Checkbox("Activate", &this->m_activate);	
		return &this->m_activate;};
	this->add_draw_item(activate_checkbox);
	
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

