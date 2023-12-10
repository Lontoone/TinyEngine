#pragma once

#ifndef BASIC_FLAGS
#define BASIC_FLAGS

const enum class EXECUTE_TIMING
{
	MAIN_LOGIC,
	BEFORE_FRAME,
	AFTER_FRAME,

};

const enum class Bind_Type 
{
	DIFFUSE,
	NORMAL,
};

const enum class LIGHT_Type
{
	DIRECTIONAL,
	POINT_LIGHT,
	SPOT_LIGHT
};

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
