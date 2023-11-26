#pragma once
#include <Fastsetup.h>

class DefaultEditorGrid

{
public:
	DefaultEditorGrid();
	DefaultEditorGrid(string vert_shader_path, string frag_shader_path);	
	Shader grid_shader;
	void render();
	float grid_count = 1000;
	
private:
	//float grid_count  = 1000;
	const float GRID_VERTS[20] = {
		- grid_count , 0 , -grid_count ,  0,0,
		- grid_count  , 0 , grid_count ,  0,1,
		grid_count  , 0 , grid_count  ,   1,1,
		grid_count  , 0 , -grid_count 	 ,1,0
	};
	const unsigned int GRID_INDEX[6] = {
		0,1,2,
		0,2,3,
	};

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};
