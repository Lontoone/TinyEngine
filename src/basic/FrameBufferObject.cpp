#include "FrameBufferObject.h"
#include <vector>

FramebufferObject::FramebufferObject(Shader* shader, GLenum& draw_buffers, const unsigned int& buffer_cnt, unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		
	vector<unsigned int> framebuffer_texture;
	framebuffer_texture.resize(buffer_cnt);

	glGenTextures(buffer_cnt, &framebuffer_texture[0]);

	for (int i = 0; i < buffer_cnt; i++) {
		glBindTexture(GL_TEXTURE_2D, framebuffer_texture[i]);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
		glFramebufferTexture(GL_FRAMEBUFFER, draw_buffers[&i], framebuffer_texture[i], 0);
	}

	glDrawBuffers(buffer_cnt, &draw_buffers);

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


}
