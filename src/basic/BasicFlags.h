#pragma once

#ifndef BASIC_FLAGS
#define BASIC_FLAGS
#include <iostream>
#include <string>

const enum class EXECUTE_TIMING
{
	MAIN_LOGIC,
	BEFORE_FRAME,
	AFTER_FRAME,

};

const enum Bind_Type 
{
	DIFFUSE,
	NORMAL,
	NONE,
};

const enum class LIGHT_Type
{
	DIRECTIONAL,
	POINT_LIGHT,
	SPOT_LIGHT
};


#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)
const static char* u_MAT_PARA_ID = "u_MAT_PARA_ID"; // diffuse
const static char* u_MAT_PARA_IS = "u_MAT_PARA_IS"; // Specular
const static char* u_MAT_PARA_IA = "u_MAT_PARA_IA"; // ambient
const static char* u_MAT_PARA_SN = "u_MAT_PARA_SN"; // Shinese

const static char* u_LIGHT_VP_MATRIX  = "u_LIGHT_VP_MATRIX";
const static char* u_PROJ_MATRIX  = "u_PROJ_MATRIX";
const static char* u_VIEW_MATRIX  = "u_VIEW_MATRIX";
const static char* u_VP_MATRIX  = "u_VP_MATRIX";
const static char* u_M_MATRIX  = "u_M_MATRIX";
const static char* u_CAM_POS  = "u_CAM_POS";

//#define u_TEX_SHADOW_MAP "u_TEX_SHADOW_MAP";
const static char* u_TEX_SHADOW_MAP = "u_TEX_SHADOW_MAP";
const static char* u_TEX_SHADOW_MAP0 = "u_TEX_SHADOW_MAP0";
const static char* u_TEX_SHADOW_MAP1 = "u_TEX_SHADOW_MAP1";
const static char* u_TEX_SHADOW_MAP2 = "u_TEX_SHADOW_MAP2";
const static char* u_TEX_SHADOW_MAP3 = "u_TEX_SHADOW_MAP3";

const static char* u_LIGHT_WORLD_POS0 = "u_LIGHT_WORLD_POS0";

/*
static const char* CODE_TO_STR( const char* str) {
	std::string st = std::string(str);
	st.erase(0, 1); // erase the first quote
	st.erase(st.size() - 1); // erase the last quote and the dot	
	return st.c_str();
}*/

/*
static const GLenum COLOR_FRAME_BUFFERS[] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2
	//GL_DEPTH_ATTACHMENT,
};

static const GLenum DEPTH_FRAME_BUFFERS[] = {	
	GL_DEPTH_ATTACHMENT,
};
*/
#endif // !BASIC_FLAGS
