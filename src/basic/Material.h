#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "Fastsetup.h"
#include "Texture.h"

using namespace std;

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
	
	//vector<Texture*> m_textures;
	/*
	Texture* m_diffuse_map;
	Texture* m_normal_map;
	*/
	Shader* m_shader = nullptr;
	void add_texture(Bind_Type _type , string _tex_path);


	virtual void render();


private:
	void load_shader();
	void set_uniform();
};


#endif // !MATERIAL_H
