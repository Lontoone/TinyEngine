#pragma once
#ifndef IndirectInstancedMesh_H
#define IndirectInstancedMesh_H

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
	void load_Position();

	vector<Vertex> m_all_vertex;
	//vector<float> m_all_uv;
	vector<unsigned int> m_all_index;
	vector<DrawElementCommand> m_draw_cmds;
	virtual void Do() override {
		
	};

};

#endif // !IndirecInstancedMesh 