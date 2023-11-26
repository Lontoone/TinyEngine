#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <map>
#include <BasicFlags.h>
#include <BasicFlags.h>

using namespace std;

/*
static const char* s_bind_types[] =
{ "DIFFUSE", "NORMAL" };
*/
static map<Bind_Type, const char*> s_bind_types = {
	{ Bind_Type::DIFFUSE , "DIFFUSE" },
	{ Bind_Type::NORMAL, "NORMAL"} 
};

class Texture {	
public :
	Texture();
	Texture(unsigned _width , unsigned int _height);
	Texture(const string _file_path);
	~Texture();

	unsigned int	m_texture_id;
	string			m_file_path;
	//const char*		m_file_path;
	int				m_width, m_height, m_channels;
	Bind_Type		m_bind_type;

	void			load_file(const string _file_path);
	void			init_texture();
	//const char*		get_file_path_char(){return this->m_file_path.c_str() ;};

private:
	unsigned char* m_data;


};

#endif