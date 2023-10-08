#pragma once
#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad/glad.h>
#include "Component.h"
#include "Material.h"

using namespace std;
using namespace glm;
struct Vertex
{
    vec3 m_pos;
    vec2 m_uv;
    vec3 m_normal;

    Vertex() {}

    Vertex(const vec3& pos, const vec2& tex, const vec3& normal)
    {
        m_pos = pos;
        m_uv = tex;
        m_normal = normal;
    }

    Vertex(const vec3& pos, const vec2& tex)
    {
        m_pos = pos;
        m_uv = tex;
        m_normal = vec3(0.0f, 0.0f, 0.0f);
    }
};

class Mesh :public Component{
public:
	Mesh();
	Mesh(const string& path);
	Mesh(const string& path , Shader& _default_shader );
	~Mesh();
	vector<Material*> materials;

	string m_srcPath = "";
	string m_srcDirecotory ="";

    struct MeshVert {
        MeshVert();
        ~MeshVert();

        void Init(const vector<Vertex>& Vertices,
            const std::vector<unsigned int>& Indices);

        GLuint VA;
        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };
	
    void Render();
	void Do() override; 
	 

private:
    vector<MeshVert> m_Entries;
	Shader m_default_shader;
    //vector<Texture*> m_Textures; //TODO: model texture

	bool LoadModel(const string& path);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	bool InitMaterials(const aiScene* pScene);//, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void Clear();
};