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

void Material::render()
{
	this->bind_texture_uniform();
	/*
	for (int i = 0; i < this->m_textures.size();i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D , this->m_textures[i].value);	
	}
	*/
	int i = 0;
	for (auto tex_ptr :this->m_textures) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D , tex_ptr.second->m_texture_id);
		i++;
	}
}

void Material::load_shader()
{
}

void Material::bind_texture_uniform()
{
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
}
