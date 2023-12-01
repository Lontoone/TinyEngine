
#include "IndirectInstancedMesh.h"

IndirectInstancedMesh::IndirectInstancedMesh(Mesh meshes[])
{
	GLuint start_index_counter = 0;
	GLuint start_vertex_counter = 0;
	
	for (int i = 0; i < sizeof(meshes) / sizeof(Mesh); i++ ) {
		for (int j = 0; j < meshes[i].m_Entries.size(); j++) {		
				
			this->add_draw_cmds( 
				meshes[i].m_Entries[j].m_vertices.size(),
				100 , 
				start_index_counter + meshes[i].m_Entries[j].m_indices.size(),
				start_vertex_counter + meshes[i].m_Entries[j].m_vertices.size(),
				0);


			this->m_all_vertex.insert(	this->m_all_vertex.end() , meshes[i].m_Entries[j].m_vertices.begin() , meshes[i].m_Entries[j].m_vertices.end());
			this->m_all_index.insert(	this->m_all_index.end(), meshes[i].m_Entries[j].m_indices.begin(), meshes[i].m_Entries[j].m_indices.end());

			start_index_counter += meshes[i].m_Entries[j].m_indices.size();
			start_vertex_counter += meshes[i].m_Entries[j].m_vertices.size();
		}
		/*
		*/	
	}

	this->m_material = *meshes[0].materials[0]; //Temp : 取第一個

	this->InitBuffers();
}

void IndirectInstancedMesh::add_draw_cmds(GLuint vertex_count, GLuint instance_count, GLuint first_index, GLuint base_vertex, GLuint base_instance)
{
	DrawElementCommand cmd ;
	cmd.base_instance = base_instance;
	cmd.base_vertex = base_vertex;
	cmd.first_index = first_index; 
	cmd.instance_counts = instance_count;
	cmd.vertex_count = vertex_count;

	this->m_draw_cmds.push_back( cmd );

}

void IndirectInstancedMesh::Do()
{
	//this->Render();
}

void IndirectInstancedMesh::Render()
{
	this->m_material.m_shader->activate();
	glBindVertexArray(this->vao);
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, this->m_all_index.size(), 0);
}

void IndirectInstancedMesh::InitBuffers()
{
	glGenVertexArrays(1 , &this->vao);
	glBindVertexArray(this->vao);

	// storage buffer
	glGenBuffers(1 , &this->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , this->ssbo);
	//glBufferStorage(GL_SHADER_STORAGE_BUFFER , sizeof(this->m_all_vertex) , &this->m_all_vertex[0], GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER , 0 , ssbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->ssbo); // Bind the buffer

	// command buffer
	glGenBuffers(1 , &this->ibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->ibo);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, sizeof(this->m_draw_cmds), &this->m_draw_cmds[0], GL_MAP_READ_BIT);

	// index buffer
	glGenBuffers(1 , &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->m_all_index), &this->m_all_index[0], GL_STATIC_DRAW);

	//TODO: VertexAttribArray....


	// end
	glBindVertexArray(0);
}
