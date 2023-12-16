#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include "Fastsetup.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class Material
{
public:
	Material();
	Material(const char* _name);
	//Material(Texture& _diff , Texture& _normal);
	~Material();
	
	string m_shader_name;
	string m_material_name;
	map<Bind_Type, Texture*> m_textures;
	
	Shader* m_shader = nullptr;
	void add_texture(Bind_Type _type , string _tex_path);

	void set_model_matrix(mat4 _mode_matrix);
	void set_uniform_matrix(map< const char* , mat4 > uniform_pairs);
	
	virtual void render();

	vec3 m_mat_para_ia;
	vec3 m_mat_para_id;  
	vec3 m_mat_para_is;
	float m_mat_para_sn;



private:
	void load_shader();
	void bind_texture_uniform();
};


#endif // !MATERIAL_H
