#pragma once

#ifndef FASET_SETUP
#define FASET_SETUP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

class Shader {
public:
	Shader();
	Shader(const string src_path, const string _shaderName);	
	~Shader();

	MechainState m_state;
	void init_program();
	void activate();
private:
	


};

#endif // FASET_SETUP
