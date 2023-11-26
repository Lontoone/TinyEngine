#include "DefaultGrid.h"

DefaultEditorGrid::DefaultEditorGrid(string vert_shader_path , string frag_shader_path)
{	
	this->grid_shader = Shader(vert_shader_path, frag_shader_path);
	this->grid_shader.add_variables("u_grid_count");


	//glVertexPointer(3, GL_FLOAT , 0 , this->GRID_VERTS);

	glGenVertexArrays(1,&this->vao);	
	glBindVertexArray(this->vao);

	glGenBuffers(1 , &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER , this->vbo);
	glBufferData(GL_ARRAY_BUFFER , sizeof(this->GRID_VERTS) , this->GRID_VERTS , GL_STATIC_DRAW );
			
	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->GRID_INDEX), this->GRID_INDEX, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)12);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void DefaultEditorGrid::render()
{	
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER , this->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

	this->grid_shader.activate();
	glUniform1f(this->grid_shader.shader_variables["u_grid_count"], this->grid_count );
	glDrawElements(GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0);
}

