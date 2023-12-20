#include "Material.h"

#define stringify( name ) #name
Material::Material()
{

}

Material::Material(const char* _name)
{
	this->m_material_name = _name;
}
/*

Material::Material(Texture& _diff, Texture& _normal)
{
	//this->m_diffuse_map = _diff;
	//this->m_normal_map = _normal;
	//TODO: Add to list
	cout << "load diff map at" << _diff.m_file_path;
}

*/
Material::~Material()
{
	cout << "deconst mat" << endl;	
	///// CLEAR TEXTURE ////
	for (auto ptr = this->m_textures.begin(); ptr != this->m_textures.end(); ++ptr) {
		delete ptr->second;
	}

	this->m_textures.clear();
}
/*
*/

void Material::add_texture(Bind_Type _type, string _tex_path)
{
	//this->m_textures
	Texture* _new_tex = new Texture(_tex_path);
	cout << "Load tex , path: " << _tex_path <<" Bind name "<< s_bind_types[_type] <<" bind add "<< _new_tex  << endl;
	this->m_textures.insert({ _type  , _new_tex });
}


void Material::set_model_matrix(mat4 _mode_matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(this->m_shader->m_state.programId, "model"), 1, GL_FALSE, value_ptr(_mode_matrix));
}

void Material::set_uniform_matrix(map<const char*, mat4> uniform_pairs)
{
	for (const auto &pair : uniform_pairs) {
		glUniformMatrix4fv(glGetUniformLocation(this->m_shader->m_state.programId, pair.first), 1, GL_FALSE, value_ptr(pair.second));
	}
}

void Material::render(bool write_gbuffer)
{

	//============================
	// §ï¦¨loop through all texture enum name...
	//============================
	int cnt = 0;
	for (int i = Bind_Type::DIFFUSE; i != Bind_Type::NONE; ++i) {
		// Bind Texture		
		Bind_Type tex_type = static_cast<Bind_Type>(i);
		char* uniform_name = new char[strlen(s_bind_types[tex_type]) + 4];
		strcpy(uniform_name, "NO_");
		strcat(uniform_name, s_bind_types[tex_type]);

		unsigned int tex = 99;
		glActiveTexture(GL_TEXTURE0 + cnt);
		// Check if texture exists , setting the uniform "NO_..."   (NO_DIFFUSE , NO_NORMAL....etc)
		if (this->m_textures.count(tex_type) && this->m_use_texture) {
			tex = this->m_textures[tex_type]->m_texture_id;
			glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, uniform_name), false);
		}
		else {
			glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, uniform_name), true);
		}
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, s_bind_types[tex_type]), cnt);

		
		// Bind Material para
		glUniform3fv(glGetUniformLocation(this->m_shader->m_state.programId, u_MAT_PARA_IA),1, value_ptr( this->m_mat_para_ia));
		glUniform3fv(glGetUniformLocation(this->m_shader->m_state.programId, u_MAT_PARA_IS), 1, value_ptr(this->m_mat_para_is));
		glUniform3fv(glGetUniformLocation(this->m_shader->m_state.programId, u_MAT_PARA_ID), 1, value_ptr(this->m_mat_para_id));
		glUniform1f(glGetUniformLocation(this->m_shader->m_state.programId, u_MAT_PARA_SN), this->m_mat_para_sn);

		glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, u_USE_SHADOW), write_gbuffer);

		cnt++;
	}
	/*
	int i = 0;
	for (auto tex_ptr :this->m_textures) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D , tex_ptr.second->m_texture_id);		
		glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, s_bind_types[tex_ptr.first]), i);
		//cout << "BIND " << s_bind_types[tex_ptr.first] <<" at "<<i;
		i++;
	}
	*/
}

void Material::load_shader()
{
}

void Material::bind_texture_uniform()
{
	/*
	if (this->m_shader != nullptr) {
		this->m_shader->activate();
		int i = 0;
		for (const auto& _tex_map : this->m_textures) {
			//glActivateTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, s_bind_types[_tex_map.first] ), i);
			i++;

			//cout<< "allocate shader: "<< this->m_shader->m_state.programId <<" texture :"<< s_bind_types[_tex_map.first] << endl;
		}
	}
	*/
	int i = 0;
	for (const auto& _tex_map : this->m_textures) {
		//glActivateTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(this->m_shader->m_state.programId, s_bind_types[_tex_map.first]), i);
		i++;
	}
}
