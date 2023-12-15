#pragma once

#ifndef FRAMEBUFFER_OBJECT_H
#define FRAMEBUFFER_OBJECT_H
#include <vector>
#include <Fastsetup.h>
class FrameBufferDebugger;
#include <FrameBufferDebugger.h>
#include <BasicFlags.h>

using namespace std;
class FramebufferObject
{
public:
	FramebufferObject() {};
	FramebufferObject(Shader* shader, const GLenum* draw_buffers,  int buffer_cnt, unsigned int& width, unsigned int& height);
	FramebufferObject(Shader* shader, const GLenum draw_target, const GLenum texture_target, const GLenum color_draw, const GLenum color_read, unsigned int& width, unsigned int& height);
	void create_shadow_buffer(Shader* shader , unsigned int& width, unsigned int& height);

	Shader* shader;
	unsigned int framebuffer_texture[10];
	unsigned int* width;
	unsigned int* height;
	
	unsigned int fbo = NULL;
	bool is_enabled = true; //ToDo:  more usage?

	const float rectangleVertices[24] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	void activate();	
	void blit(unsigned int src_id, FramebufferObject& fbo);
	void blit(unsigned int src_texture_id  , unsigned int dst_fbo);
	void blit(unsigned int src_texture_id, unsigned int dst_fbo , Shader& shader);
	void blit(unsigned int src_texture_id, unsigned int dst_fbo, unsigned int additional_texture);
	void blit(unsigned int src_texture_id, FramebufferObject& fbo, unsigned int additional_texture);
	void blit(unsigned int src_texture_id, FramebufferObject& fbo, const unsigned int additional_textures[]);
	void set_frame_uniform();
	void update_debugger(unsigned int& texture_idx);
	
	
	//TODO: Make them shared.
	unsigned int rbo ;
	unsigned int rectVAO , rectVBO ;
	FrameBufferDebugger* frame_debugger;

private:
};


#endif // !FRAMEBUFFER_OBJECT_H
