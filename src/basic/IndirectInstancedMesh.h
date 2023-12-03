#pragma once
#ifndef IndirectInstancedMesh_H
#define IndirectInstancedMesh_H

#ifndef ENV_PATH_H
#include <env_path.h>
#endif
/*
#include <GL/glew.h>
#include <GLFW/glfw3.h>
*/
#include <GL/glew.h>

#include <Mesh.h>
#include <iostream>
#include <format>
#include <vector>
#include <Component.h>

// ====== For HW3 =======
#include <SpatialSample.h>
using namespace INANOA::SCENE::EXPERIMENTAL;
//=======================

class Shader;

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
	vector<DrawElementCommand> m_draw_cmds;
	//DrawElementCommand m_draw_cmds[2];
	Material m_material;
	Shader cs_reset_shader;
	Shader cs_view_culling_shader;

	IndirectInstancedMesh(vector<Mesh>& meshes);
	void load_position();
	void add_draw_cmds(GLuint vertex_count , GLuint instance_count , GLuint first_index , GLuint base_vertex , GLuint base_instance);
	

	// Component Function override
	void Do() override;
	void DO_Before_Frame() override;
	void Do_End_Frame() override {};
	void Render();

private:
	GLuint vao;
	GLuint vbo;
	GLuint ssbo_draw;
	GLuint ssbo_offset;
	GLuint ebo;
	GLuint ibo; // indirect buffer object

	//vector<vec4>			m_all_vertex;
	vector<Vertex>			m_all_vertex;
	//vector<float> m_all_uv;
	vector<unsigned int>	m_all_index;
	vector<vec4>			m_all_offset;
	vector<vec4>			m_all_visiable_offset;

	void init_buffers();
	void init_compute_shader();

};

#endif // !IndirecInstancedMesh 