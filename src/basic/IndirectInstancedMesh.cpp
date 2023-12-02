
#include "IndirectInstancedMesh.h"

#pragma region TEST_CODE
float datas[] = {
	//Position					//Color
	-0.05f,  0.05f, 0.0f,0,     1,0,0,1,
	0.05f, -0.05f, 0.0f,0,     0,1,0,1,
	-0.05f, -0.05f, 0.0f,0,     0,0,1,1,
	-0.05f,  0.05f, 0.0f,0,     1,0,0,1,
	0.05f, -0.05f, 0.0f,0,     0,1,0,1,
	0.05f,  0.05f, 0.0f,0,     0,1,1,1,

	-0.05f,  0.05f, 0.0f,0,     0.5,0.5,0.5,1,
	0.07f, -0.09f, 0.0f,0,     0.5,0.5,0.5,1,
	-0.05f, -0.05f, 1.0f,0,    0.5,0.5,0.5,1
};

vector<float> data_f = {
	//Position					//Color
	-0.05f,  0.05f, 0.0f,0,     1,0,0,1,
	0.05f, -0.05f, 0.0f,0,     0,1,0,1,
	-0.05f, -0.05f, 0.0f,0,     0,0,1,1,
	-0.05f,  0.05f, 0.0f,0,     1,0,0,1,
	0.05f, -0.05f, 0.0f,0,     0,1,0,1,
	0.05f,  0.05f, 0.0f,0,     0,1,1,1,

	-0.05f,  0.05f, 0.0f,0,     0.5,0.5,0.5,1,
	0.07f, -0.09f, 0.0f,0,     0.5,0.5,0.5,1,
	-0.05f, -0.05f, 1.0f,0,    0.5,0.5,0.5,1
};



unsigned int combined_idx[] = {
	0,1,2,
	3,4,5,

	0,1,2
};
const int NUM_DRAWS = 2;
int translations[NUM_DRAWS];

unsigned int vvao, vvbo,eebo ,indr_id;

// ���Ǥ����
struct DrawCmd {
	GLuint count;    //���I��
	GLuint instance_counts; //instance��
	GLuint first_index;
	GLuint base_vertex;
	GLuint base_instance;
};

void test_init() {
	glGenVertexArrays(1, &vvao);
	glBindVertexArray(vvao);

	glGenBuffers(1, &vvbo);
	//glUseProgram(cs_reset_p);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vvbo);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(datas), datas, GL_DYNAMIC_STORAGE_BIT); //��data��	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vvbo);
	//cmd buffer	
	DrawCmd drawcmd[NUM_DRAWS];
	drawcmd[0].count = 6;
	drawcmd[0].instance_counts = 3;
	drawcmd[0].first_index = 0;
	drawcmd[0].base_vertex = 0;
	drawcmd[0].base_instance = 0;
	

	drawcmd[1].count = 3;
	drawcmd[1].instance_counts = 3;
	drawcmd[1].first_index = 6;
	drawcmd[1].base_vertex = 6;
	drawcmd[1].base_instance = 0;
	
	glGenBuffers(1, &indr_id);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indr_id);
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawCmd), &drawcmd[0], GL_MAP_READ_BIT); //��cmd��

	//ssbo
	glBindBuffer(GL_ARRAY_BUFFER, vvbo); // Bind the buffer
	glEnableVertexAttribArray(0);
	// Attri
	glVertexAttribPointer(
		0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(float)*8,        // stride
		(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(float) * 8,       // stride
		(void*)16 // array buffer offset
	);

	//ebo
	glGenBuffers(1, &eebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_DRAWS * sizeof(combined_idx), combined_idx, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void test_draw() {	
	
	glBindVertexArray(vvao);	
	//glBindBuffer(GL_ARRAY_BUFFER, vvbo);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vvbo);

	//glDispatchCompute(1, 1, 1);
	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//glUseProgram(p);
	//glDrawArraysIndirect(GL_TRIANGLES, 0);
	//glMultiDrawArraysIndirect(GL_TRIANGLES, 0, NUM_DRAWS, 0); 
	//glDrawElementsIndirect(GL_TRIANGLES , GL_UNSIGNED_INT ,0 );
	//glDrawArrays(GL_TRIANGLES, 0 , 9 );
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, NUM_DRAWS, 0);
}


#pragma endregion

//int DRAW_COUNT = 10000;
int DRAW_COUNT[3];  // For HW3

vector<vec4> hw3_load_position() {
	vector<vec4> offset;
	/*
	for (int i = 0; i < count; i++) {
		offset.push_back(vec4( i % 10 , 0, i/10+i , 1));
		//offset.push_back(vec4(i *0.5, 0, i * 0.5, 1));
	}
	*/
	float x_offset = 30;
	float z_offset = 200;

	SpatialSample* sp0 = SpatialSample::importBinaryFile(GET_SRC_FOLDER() + string("\\assets\\poissonPoints_155304s.ss2"));
	SpatialSample* sp1 = SpatialSample::importBinaryFile(GET_SRC_FOLDER() + string("\\assets\\poissonPoints_1010s.ss2"));
	SpatialSample* sp2 = SpatialSample::importBinaryFile(GET_SRC_FOLDER() + string("\\assets\\poissonPoints_2797s.ss2"));
	int num_sp = sp0->numSample();
	for (int idx = 0; idx < num_sp; idx++) {
		const float* pos = sp0->position(idx);
		offset.push_back(vec4(pos[0]+ x_offset, pos[1], pos[2]+ z_offset, 1));
	}
	DRAW_COUNT[0] = num_sp;
	
	num_sp = sp1->numSample();
	for (int idx = 0; idx < num_sp ; idx++) {
		const float* pos = sp1->position(idx);
		offset.push_back(vec4(pos[0] + x_offset, pos[1], pos[2] + z_offset, 1));
	}
	DRAW_COUNT[1] = num_sp;

	num_sp = sp2->numSample();
	for (int idx = 0; idx < num_sp; idx++) {
		const float* pos = sp2->position(idx);
		offset.push_back(vec4(pos[0] + x_offset, pos[1], pos[2] + z_offset, 1));
	}
	DRAW_COUNT[2] = num_sp;
	return offset;
}

//void merge_entries(Mesh& targetMesh ,vector<vec4>& verteices , vector<unsigned int>& index ) {
void merge_entries(Mesh& targetMesh, vector<Vertex>& verteices, vector<unsigned int>& index) {

	unsigned int base_index_counter = 0;
	for (int i = 0; i < targetMesh.m_Entries.size(); i++) {
		//verteices.insert(verteices.end(), targetMesh.m_Entries[i].m_vertices.begin(), targetMesh.m_Entries[i].m_vertices.end());
		verteices.insert(verteices.end(), targetMesh.m_Entries[i].m_vertexs.begin(), targetMesh.m_Entries[i].m_vertexs.end());

		for (int j = 0; j < targetMesh.m_Entries[i].m_indices.size(); j++) {
			index.push_back(targetMesh.m_Entries[i].m_indices[j] + base_index_counter);
		}
		base_index_counter += targetMesh.m_Entries[i].m_vertices.size();

	}
}


IndirectInstancedMesh::IndirectInstancedMesh(Mesh meshes[])
{
	GLuint start_index_counter = 0;
	GLuint start_vertex_counter = 0;

	for (int i = 0; i < sizeof(*meshes) / sizeof(Mesh); i++ ) {
		// Merge entry's points
		vector<Vertex> vert;
		vector<unsigned int> index;
		merge_entries(meshes[i] , vert, index);
		this->add_draw_cmds( 
			index.size(),  //vertex count
			DRAW_COUNT[i], //instance count
			start_index_counter , //first index
			start_vertex_counter , //base vertex
			0
		);
		this->m_all_vertex.insert(this->m_all_vertex.end(), vert.begin(), vert.end());
		this->m_all_index.insert(this->m_all_index.end(), index.begin(), index.end());

		start_index_counter += index.size();
		start_vertex_counter += vert.size();
	}
	
	//TODO: �� texture array
	this->m_material = *meshes[0].materials[meshes[0].materials.size()-1]; 

	this->init_buffers();
	this->init_compute_shader();

	//test_init();
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

void IndirectInstancedMesh::init_compute_shader()
{
	//static string src_path = string(EXPAND(_PRJ_SRC_PATH)) + string("\\assets\\shaders\\hw3_reset_cs.glsl");	
	this->cs_reset_shader =  Shader(GET_SRC_FOLDER() + string("\\assets\\shaders\\hw3_reset_cs.glsl"));
	glUniform1f(glGetUniformLocation(this->cs_reset_shader.m_state.programId, "MAX_COUNT"), this->m_all_offset.size());
	this->cs_view_culling_shader = Shader(GET_SRC_FOLDER() + string("\\assets\\shaders\\hw3_view_culling.glsl"));
	glUniform1f(glGetUniformLocation(this->cs_view_culling_shader.m_state.programId, "MAX_COUNT"), this->m_all_offset.size());

}

void IndirectInstancedMesh::Do()
{
	this->Render();
	//test_draw();
}

float debug_time = 0;
void IndirectInstancedMesh::DO_Before_Frame()
{
	//Reset draw buffer
	glBindVertexArray(this->vao);
	this->cs_reset_shader.activate();
	//glUniform1f(glGetUniformLocation(this->cs_reset_shader.m_state.programId, "time"), debug_time); // debug purpose	
	glDispatchCompute(1, 2, 20);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	debug_time += 0.001f;

}

void IndirectInstancedMesh::Render()
{	
	// collect
	/*	
	*/
	glBindVertexArray(this->vao);
	this->cs_view_culling_shader.activate();
	glDispatchCompute(1, 1, 10);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// basic render
	this->m_material.m_shader->activate();
	this->m_material.render();
	glBindVertexArray(this->vao);
	//glBindBuffer(GL_ARRAY_BUFFER, this->ssbo_offset); // Bind the buffer
#pragma region TEST 
	//glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, this->m_all_index.size(), 0);
	//glDrawElementsInstanced(GL_TRIANGLES , m_all_index.size() , GL_UNSIGNED_INT , 0 , 10 );
	//glDrawElements(GL_TRIANGLES , m_all_index.size() , GL_UNSIGNED_INT , 0);
	//glDrawElementsInstancedBaseVertex();
	//glMultiDrawArraysIndirect(GL_TRIANGLES , 0  , m_all_index.size() , 0);
	//glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, this->m_draw_cmds.size(), 0);
	//glDrawArrays(GL_TRIANGLES , 0 , 48);
	/*
	*/	
	//glDrawArraysIndirect(GL_TRIANGLES,  0);
	//glDrawElementsIndirect(GL_TRIANGLES , GL_UNSIGNED_INT , 0);
	//glDrawElements(GL_TRIANGLES , this->m_all_index.size(), GL_UNSIGNED_INT, 0);

#pragma endregion

	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, this->m_draw_cmds.size(), 0);
	
	glBindVertexArray(0);
}

void IndirectInstancedMesh::init_buffers()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->ibo);
	//glBufferStorage(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawCmd), &drawcmd[0], GL_MAP_READ_BIT); //��cmd��
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, m_draw_cmds.size() * sizeof(DrawElementCommand), &this->m_draw_cmds[0], GL_MAP_READ_BIT); //��cmd��
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->ibo);

	// vertex buffer 
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo); // Bind the buffer
	glBufferData(GL_ARRAY_BUFFER, this->m_all_vertex.size() * sizeof(Vertex) , &this->m_all_vertex[0] , GL_STATIC_DRAW);

	
	// all offset -> ssbo
	this->m_all_offset= hw3_load_position( );
	this->m_all_visiable_offset.resize(this->m_all_offset.size());

	glGenBuffers(1, &this->ssbo_offset);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo_offset); // Bind the buffer
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, this->m_all_offset.size() * sizeof(float) * 4, &this->m_all_offset[0], GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo_offset);
	glBindBuffer(GL_ARRAY_BUFFER, this->ssbo_offset); // Bind the buffer

	// only the draw target offset -> ssbo
	glGenBuffers(1, &this->ssbo_draw);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo_draw); // Bind the buffer
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, this->m_all_visiable_offset.size() * sizeof(float) * 4, &this->m_all_visiable_offset[0], GL_MAP_READ_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->ssbo_draw);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, this->ssbo_draw);
	/*
	*/
	
	/*
	//glUseProgram(cs_reset_p);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
	//glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(datas), datas, GL_DYNAMIC_STORAGE_BIT); //��data��	
	//glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float)* data_f.size() ,&data_f[0], GL_DYNAMIC_STORAGE_BIT); //��data��	
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, this->m_all_vertex.size() * sizeof(vec4), &this->m_all_vertex[0], GL_DYNAMIC_STORAGE_BIT); //��data��	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);
	*/
	/*
	*/
	//cmd buffer	
	/*
	DrawCmd drawcmd[NUM_DRAWS];
	drawcmd[0].count = 6;
	drawcmd[0].instance_counts = 3;
	drawcmd[0].first_index = 0;
	drawcmd[0].base_vertex = 0;
	drawcmd[0].base_instance = 0;


	drawcmd[1].count = 3;
	drawcmd[1].instance_counts = 3;
	drawcmd[1].first_index = 6;
	drawcmd[1].base_vertex = 6;
	drawcmd[1].base_instance = 0;

	*/
	
#pragma region LAYOUT
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glEnableVertexAttribArray(0);
	//==== Attri =====
	// vertex
	glVertexAttribPointer(
		0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		//sizeof(float) * 4,        // stride
		sizeof(Vertex),
		(void*)0 // array buffer offset
	);

	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?		
		sizeof(Vertex),
		(void*)16 // array buffer offset
	);
	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?		
		sizeof(Vertex),
		(void*)32 // array buffer offset
	);
	// tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?		
		sizeof(Vertex),
		(void*)48 // array buffer offset
	);

	// Offset
	/*
	glEnableVertexAttribArray(10);
	//glBindBuffer(GL_ARRAY_BUFFER, this->ssbo_offset);
	glBindBuffer(GL_ARRAY_BUFFER, this->ssbo_draw);
	glVertexAttribPointer(
		10,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,        // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(float) * 4,        // stride
		(void*)0 // array buffer offset
	);
	glVertexAttribDivisor(10,1);*/
	
#pragma endregion

	//ebo
	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_DRAWS * sizeof(combined_idx), combined_idx, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->m_all_index.size(), &this->m_all_index[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}
