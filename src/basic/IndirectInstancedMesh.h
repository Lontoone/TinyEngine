#pragma once
#ifndef IndirectInstancedMesh_H
#define IndirectInstancedMesh_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Mesh.h>
#include <iostream>
#include <vector>
#include <Component.h>

class IndirectInstancedMesh: public UiableComponent {

struct DrawElementCommand
{
	GLuint vertex_count;
	GLuint instance_counts;
	GLuint first_index;
	GLuint base_vertex;   //index ¤~­n : The value added to the vertex index before indexing into the vertex buffer
	GLuint base_instance;
};

public :
	IndirectInstancedMesh(Mesh meshes[]);
	void load_position();
	void add_draw_cmds(GLuint vertex_count , GLuint instance_count , GLuint first_index , GLuint base_vertex , GLuint base_instance);

	vector<DrawElementCommand> m_draw_cmds;

	Material m_material;
	void Do() override;
	void Render();

private:
	GLuint vao;
	GLuint ssbo;
	GLuint ebo;
	GLuint ibo; // indirect buffer object

	vector<Vertex>			m_all_vertex;
	//vector<float> m_all_uv;
	vector<unsigned int>	m_all_index;

	void InitBuffers();

};

#endif // !IndirecInstancedMesh 