#pragma once
#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GL/glew.h>
//#include <GL/glew.h>
#include "LightingManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Material.h"

using namespace std;
using namespace glm;
struct Vertex
{
public:
    vec4 m_pos;
    vec4 m_uv;
    vec4 m_normal;
    vec4 m_tangent;

    Vertex() {}

    Vertex(const vec3& pos, const vec2& tex, const vec3& normal )
    {
        m_pos = vec4( pos , 1.0f);
        m_uv = vec4(tex,0,0);
        m_normal =  vec4(normal,1.0);
    }

    Vertex(const vec3& pos, const vec2& tex)
    {
        m_pos = vec4(pos, 1.0f);
        m_uv = vec4(tex, 0, 0);
        m_normal = vec4(0.0f, 0.0f, 0.0f,0.0f);
    }
};

class Mesh : public UiableComponent {
public:
	Mesh();
	Mesh(const string& path);
	Mesh(const string& path , Shader& _default_shader );
    virtual ~Mesh();
	vector<Material*> materials;

	string m_srcPath = "";
	string m_srcDirecotory ="";

    //vector<Vertex> m_vertices;
    //vector<unsigned int> m_baseVertex;

    struct MeshVert {
        MeshVert();
        ~MeshVert();

        void Init(const vector<Vertex>& Vertices,
            const std::vector<unsigned int>& Indices);
        void CalculateTangent(vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);
        void FlashVertexData();  // You should clear vertices and index data after binding them to GPU.

        GLuint VA;
        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;

        vector<vec4> m_vertices;
        vector<Vertex> m_vertexs;
        vector<unsigned int> m_indices;
    };
    vector<MeshVert> m_Entries;
	
    void flash_entity_data(); // TODO....
    void Render();
    void Render(map<const char*, mat4> uniform_pairs);
    void Render(map<const char*, mat4> uniform_pairs , Shader &shader);
	void Do() ; 
    
    // Override for UI
    Component* copy() override;
    void init_ui_content() override;
	 

private:
	Shader m_default_shader;
    //vector<Texture*> m_Textures; //TODO: model texture

	bool LoadModel(const string& path);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	bool InitMaterials(const aiScene* pScene);//, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();
};