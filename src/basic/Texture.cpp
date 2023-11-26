#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture()
{
	this->m_file_path = "";
}

Texture::Texture(unsigned _width, unsigned int _height)
{
	glGenTextures(1, &this->m_texture_id);
	this->m_width = _width;
	this->m_height= _height;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

Texture::Texture(const string _file_path)
{
	this->load_file(_file_path);
	this->m_file_path = _file_path;
	cout <<" texture const" << m_file_path << endl;
}

Texture::~Texture()
{
	cout << "free texture " << this->m_texture_id << endl;	
	//stbi_image_free(this->m_data);
	//TODO: unbind texture (?)
}

void Texture::load_file(const string _file_path)
{
	glGenTextures(1, &this->m_texture_id);
	glBindTexture(GL_TEXTURE_2D, this->m_texture_id);
	//TODO: custom flag
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true); //set stbi vertical axis the same as gl.	
	this->m_data = stbi_load(
		_file_path.c_str()	, 
		&this->m_width		,
		&this->m_height		,
		&this->m_channels	, 0);
	this->init_texture();
	cout << "loaded texture "<< this->m_texture_id <<" " << _file_path << " w,h = " << this->m_width << " " << this->m_height << endl;
}

void Texture::init_texture()
{
	if (this->m_data) {
		if(this->m_channels ==3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->m_data);
		
		else if (this->m_channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_width, this->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(this->m_data);
	}
	else
	{
		cout << "Texture Load Failed" << endl;
	}
}

