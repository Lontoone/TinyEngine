#include "LightingManager.h"

void LightingManager::render_to_shadowmap()
{
	vector<Light*>* light_list = &Hierarchy::instance().m_ligths;
	for (auto light : *light_list)
	{
		//light->shadow_pass();
		map<const char*, mat4> uniform_pairs;
		//uniform_pairs.insert({ "view" , light->get_light_view_matrix()});
		//uniform_pairs.insert({ "projection" , light->get_projection_matrix()});		
		uniform_pairs.insert({ "u_LIGHT_VP_MATRIX" , light->get_projection_matrix()* light->get_light_view_matrix()});
		for (auto renderer: Hierarchy::instance().m_meshes) {
			renderer->get_gameobject()->m_transform->Do();			
			renderer->Render(uniform_pairs);
		}
		//Hierarchy::instance().execute(EXECUTE_TIMING::MAIN_LOGIC); //ToDo: Replace with render only function
	}
}

void LightingManager::request_shadowmap_uniform(Shader& active_shader)
{
	active_shader.activate();

	//ToDo: Find the cloest light instead of looping all the light
	vector<Light*>* light_list = &Hierarchy::instance().m_ligths;
	//for (int i = 0 ; i < light_list->size(); i++)
	int i = 0;
	for (auto light : *light_list)
	{
		//const char* loc_name = CODE_TO_STR(EXPAND(u_TEX_SHADOW_MAP));
		const char* loc_name = "u_TEX_SHADOW_MAP";
		// Light fbo contains depth :
		//glUniform1d(active_shader.shader_variables[loc_name], light->fbo->framebuffer_texture[0]);
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, light->fbo->framebuffer_texture[0]);
		glUniform1d(glGetUniformLocation( active_shader.m_state.programId  ,loc_name), i+2);  // Shadow map start from 2
		++i;
	}
}
