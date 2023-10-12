#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <string>

using namespace std;
enum Bind_Type
{
	DIFFUSE,
	NORMAL
};
static const char* s_bind_types[] =
{ "DIFFUSE", "NORMAL" };


class Texture {	
public :
	Texture();
	Texture(const string& _file_path);
	~Texture();

	unsigned int	m_texture_id;
	string			m_file_path;
	int				m_width, m_height, m_channels;
	Bind_Type		m_bind_type;

	void			load_file(const string& _file_path);
	void			init_texture();

private:
	unsigned char* m_data;


};

#endif