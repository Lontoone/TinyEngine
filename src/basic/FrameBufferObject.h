#pragma once
#include <Fastsetup.h>
#include <vector>
#ifndef FRAMEBUFFER_OBJECT_H
#define FRAMEBUFFER_OBJECT_H

using namespace std;
class FramebufferObject
{
public:
	FramebufferObject(Shader* shader, const GLenum* draw_buffers,  int buffer_cnt, int width, int height);
	Shader* shader;
	unsigned int framebuffer_texture[50];
	unsigned int width, height;
	
	unsigned int fbo = NULL;
	

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
	void blit( unsigned int src_texture_id  , unsigned int dst_fbo);
	void blit(unsigned int src_texture_id, unsigned int dst_fbo, unsigned int additional_texture);
	void blit(unsigned int src_texture_id, unsigned int dst_fbo, const unsigned int additional_textures[]);

	
	//TODO: Make them shared.
	unsigned int rbo ;
	unsigned int rectVAO , rectVBO ;

private:
};


#endif // !FRAMEBUFFER_OBJECT_H
