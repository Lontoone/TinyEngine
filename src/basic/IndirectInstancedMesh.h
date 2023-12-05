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
#include <random>
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
	Shader indirect_render_shader;

	/*
	struct IndirectDrawOffsetData
	{
		vec4 offset;
		vec4 eaten;
		IndirectDrawOffsetData();
		IndirectDrawOffsetData(vec4 _offset) {
			offset = _offset;
			eaten = vec4(0);
		};
	};
	*/

	IndirectInstancedMesh(vector<Mesh>& meshes);
	void load_position();
	void add_draw_cmds(GLuint vertex_count , GLuint instance_count , GLuint first_index , GLuint base_vertex , GLuint base_instance);
	
	void hw3_update_dog_position(vec3 pos);
	void dispatch_reset();
	void dispatch_culling();

	// Component Function override
	void Do() override;
	void DO_Before_Frame() override;
	void Do_End_Frame() override {};
	void Render();

	GLuint vao;
private:
	GLuint vbo;
	GLuint ssbo_draw;
	GLuint ssbo_offset;
	GLuint ssbo_additional; //for hw3
	GLuint ebo;
	GLuint ibo; // indirect buffer object
	GLuint textures_id;


	//vector<vec4>			m_all_vertex;
	vector<Vertex>			m_all_vertex;
	//vector<float> m_all_uv;
	vector<unsigned int>	m_all_index;
	vector<vec4>			m_all_offset;	
	vector<vec4>			m_all_visiable_offset;
	vector<vec4>			m_all_state; // scale (vec3) , eaten (1)

	void init_buffers();
	void init_compute_shader();
	void hw3_init_textures();

};

#endif // !IndirecInstancedMesh 