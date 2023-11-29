#include "IndirectInstancedMesh.h"

IndirectInstancedMesh::IndirectInstancedMesh(Mesh meshes[])
{
	for (int i = 0; i < sizeof(meshes) / sizeof(Mesh); i++ ) {
		/*
		for (int j = 0; j < meshes[i].m_Entries.size(); j++) {
			this->m_all_vertex.push_back(meshes[i].m_Entries[j]);
		}
		*/
	
		//this->m_all_vertex.insert(this->m_all_vertex.end() , meshes[i].m_vertices.begin() , meshes[i].m_vertices.end());
		//this->m_all_index.insert(this->m_all_index.end(), meshes[i].m_indices.begin(), meshes[i].m_indices.end());
	}
}
