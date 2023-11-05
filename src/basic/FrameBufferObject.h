#pragma once
#include <Fastsetup.h>
#ifndef FRAMEBUFFER_OBJECT_H
#define FRAMEBUFFER_OBJECT_H


class FramebufferObject
{
public:
	FramebufferObject(Shader* shader, GLenum& draw_buffers, const unsigned int& buffer_cnt, unsigned int width, unsigned int height);
	unsigned int fbo;
	static unsigned int rbo;

private:

};


#endif // !FRAMEBUFFER_OBJECT_H
