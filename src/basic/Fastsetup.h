#pragma once

#ifndef FASET_SETUP
#define FASET_SETUP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
struct MechainState
{
	GLuint programId;
	GLuint vertShaderId;
	GLuint fragShaderId;

};

void SetProgram(MechainState& mechainState, const string vs_file, const string fs_file);
void SetProgramFromSource(MechainState& mechainState, const char* vs, const char* fs);

class Shader {
public:
	Shader();	
	Shader(const string src_path, const string _shaderName);	
	~Shader();

	map<const char*, GLuint> shader_variables = {
		{"u_time" , 0} ,
		{"u_texelsize" , 0} , 
		{"u_mouse_position" , 0} ,
	};

	void UpdateFromSource(const string vs, const string fs);
	MechainState m_state;
	void init_variables();
	void activate();
private:
	


};

#endif // FASET_SETUP
