#include "FrameBufferObject.h"
#include <vector>
//
// buffer_cnt : GL_COLOR_ATTACHMENTi  array , define color to draw
//
FramebufferObject::FramebufferObject(Shader* shader, const GLenum* draw_buffers, int buffer_cnt, unsigned int& width, unsigned int& height)
{
	this->shader = shader;
	this->width = &width;
	this->height = &height;
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		
	
	glGenTextures(buffer_cnt, &framebuffer_texture[0]);

	for (int i = 0; i < buffer_cnt; i++) {
		glBindTexture(GL_TEXTURE_2D, this->framebuffer_texture[i]);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
		glFramebufferTexture(GL_FRAMEBUFFER, draw_buffers[i], this->framebuffer_texture[i], 0);
	}

	glDrawBuffers(buffer_cnt, draw_buffers);

	if (this->rbo == NULL) {
		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	}

	auto fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "frame buffer error " << fbo_status << endl;
	}

	// Prepare framebuffer rectangle VBO and VAO	
	if (this->rectVAO == NULL) {
		glGenVertexArrays(1, &this->rectVAO);
		glGenBuffers(1, &this->rectVBO);
		glBindVertexArray(this->rectVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	glUniform1i(glGetUniformLocation(this->shader->m_state.programId, "screenTexture"), 0);
}

FramebufferObject::FramebufferObject(Shader* shader, const GLenum draw_target, const GLenum texture_target , const GLenum color_draw , const GLenum color_read, unsigned int& width, unsigned int& height)
{
	// Create Only 1 Textures.
	this->shader = shader;
	this->width = &width;
	this->height = &height;
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	
	glGenTextures(1, &framebuffer_texture[0]);

	glBindTexture(GL_TEXTURE_2D, this->framebuffer_texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, texture_target, width, height, 0, texture_target, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	glFramebufferTexture(GL_FRAMEBUFFER, draw_target, this->framebuffer_texture[0], 0);
	
	glDrawBuffer(color_draw);
	glReadBuffer(color_read);

	if (this->rbo == NULL) {
		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	}

	auto fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "frame buffer error " << fbo_status << endl;
	}

	// Prepare framebuffer rectangle VBO and VAO	
	if (this->rectVAO == NULL) {
		glGenVertexArrays(1, &this->rectVAO);
		glGenBuffers(1, &this->rectVBO);
		glBindVertexArray(this->rectVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	glUniform1i(glGetUniformLocation(this->shader->m_state.programId, "screenTexture"), 0);
}



void FramebufferObject::activate()
{
	this->shader->activate();
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void FramebufferObject::blit(unsigned int src_id, FramebufferObject& fbo )
{
	this->blit(src_id, fbo.fbo);
	this->update_debugger(fbo.framebuffer_texture[0]);
	//this->update_debugger(src_id);
}
void FramebufferObject::blit(unsigned int src_id, unsigned int dst_fbo_id)
{

	glBindFramebuffer(GL_FRAMEBUFFER, dst_fbo_id);
	this->shader->activate();
	this->set_frame_uniform();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->rectVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, src_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FramebufferObject::blit(unsigned int src_texture_id, unsigned int dst_fbo, unsigned int additional_texture)
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, dst_fbo );
	this->shader->activate();
	this->set_frame_uniform();
	glBindVertexArray(this->rectVAO);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, src_texture_id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, additional_texture);

	// Get the location of the uniforms	
	GLint texture1Location = glGetUniformLocation(this->shader->m_state.programId, "screenTexture");
	GLint texture2Location = glGetUniformLocation(this->shader->m_state.programId, "texture2");

	// Set the uniforms to the texture units
	glUniform1i(texture1Location, 0); // Texture unit 0
	glUniform1i(texture2Location, 1); // Texture unit 1

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void FramebufferObject::blit(unsigned int src_texture_id, FramebufferObject& fbo,const unsigned int additional_textures[])
{	

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
	this->shader->activate();
	this->set_frame_uniform();
	glBindVertexArray(this->rectVAO);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, src_texture_id);

	int length = sizeof(additional_textures) / sizeof(additional_textures[0]);
	for (int i = 1; i <= length; i++) {
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, additional_textures[i-1]);		
		GLint texture2Location = glGetUniformLocation(this->shader->m_state.programId, "texture"+(i));
		glUniform1i(texture2Location, i); 
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);
	this->update_debugger(fbo.framebuffer_texture[0]);

}

void FramebufferObject::set_frame_uniform()
{
	glUniform1f(this->shader->shader_variables["u_time"], glfwGetTime() );
	glUniform2i(this->shader->shader_variables["u_texturesize"], *this->width, *this->height);
	glUniform2f(this->shader->shader_variables["u_texelsize"], 1.0f / *this->width, 1.0f / *this->height);
}

void FramebufferObject::update_debugger(unsigned int& texture_idx)
{
	if (this->frame_debugger != nullptr) {
		this->frame_debugger->Draw_Single_Frame(texture_idx);
	}
}

void FramebufferObject::blit(unsigned int src_texture_id, FramebufferObject& fbo, unsigned int additional_texture)
{

	this->blit(src_texture_id, fbo.fbo, additional_texture);
	//this->update_debugger(fbo.framebuffer_texture[0]);
	this->update_debugger(src_texture_id);


}
