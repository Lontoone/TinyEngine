#include "FrameBufferObject.h"
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
	this->m_texture_count = buffer_cnt;
	
	glGenTextures(buffer_cnt, &framebuffer_texture[0]);

	for (int i = 0; i < buffer_cnt; i++) {
		glBindTexture(GL_TEXTURE_2D, this->framebuffer_texture[i]);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);	

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
		glFramebufferTexture(GL_FRAMEBUFFER, draw_buffers[i], this->framebuffer_texture[i], 0);
	}

	glDrawBuffers(buffer_cnt, draw_buffers);
	if (this->rbo == NULL) {
		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	}
	/*
	*/

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

	glTexImage2D(GL_TEXTURE_2D, 0, texture_target, width, height, 0, texture_target, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, draw_target, GL_TEXTURE_2D,this->framebuffer_texture[0], 0);
	
	glDrawBuffer(color_draw);
	glReadBuffer(color_read);
	if (this->rbo == NULL) {
		glGenRenderbuffers(1, &this->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	}
	/*
	*/

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

void FramebufferObject::create_shadow_buffer(Shader* shader, unsigned int& width, unsigned int& height)
{
	// Create Only 1 Textures.
	this->shader = shader;
	this->width = &width;
	this->height = &height;
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


	glGenTextures(1, &framebuffer_texture[0]);

	glBindTexture(GL_TEXTURE_2D, this->framebuffer_texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->framebuffer_texture[0], 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	auto fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "frame buffer error " << fbo_status << endl;
	}

	/*
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
	*/
	//const char* shadow_map = u_TEX_SHADOW_MAP;
	//glUniform1i(glGetUniformLocation(this->shader->m_state.programId, shadow_map), 0); //u_TEX_SHADOW_MAP
}

void FramebufferObject::create_pointLight_shadow_buffer(Shader* shader, unsigned int& width, unsigned int& height)
{
	this->shader = shader;
	this->width = &width;
	this->height = &height;
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


	glGenTextures(1, &framebuffer_texture[0]);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->framebuffer_texture[0]);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->framebuffer_texture[0], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->framebuffer_texture[0], 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	auto fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "frame buffer error " << fbo_status << endl;
	}
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
	this->blit(src_id, dst_fbo_id, *this->shader);
}
void FramebufferObject::blit(unsigned int src_texture_id, unsigned int dst_fbo, Shader& shader) {

	glBindFramebuffer(GL_FRAMEBUFFER, dst_fbo);
	shader.activate();
	this->set_frame_uniform();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->rectVAO);	
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, src_texture_id);
	glUniform1i(glGetUniformLocation(shader.m_state.programId, "screenTexture"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void FramebufferObject::blit(unsigned int src_texture_id, unsigned int dst_fbo, unsigned int additional_texture)
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, dst_fbo );
	this->shader->activate();
	this->set_frame_uniform();
	glBindVertexArray(this->rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded

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

void FramebufferObject::blit(unsigned int src_texture_id, unsigned int dst_fbo_id, unsigned int additional_textures[], int count , Shader& shader)
{	

	//glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, dst_fbo_id);
	shader.activate();
	glBindVertexArray(this->rectVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, src_texture_id);

	for (int i = 1; i <= count; i++) {
		glActiveTexture(GL_TEXTURE0+i);
		//cout << "Bind texture " << additional_textures[i - 1] << endl;
		glBindTexture(GL_TEXTURE_2D, additional_textures[i-1]);		

		string bind_name_full =  (string("texture") + to_string(i));
		const char* b = bind_name_full.c_str();

		GLint texture2Location = glGetUniformLocation(shader.m_state.programId, b);
		glUniform1i(texture2Location, i); 
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);
	//this->update_debugger(fbo.framebuffer_texture[0]);

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
