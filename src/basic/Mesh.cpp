#include "Mesh.h"

Mesh::Mesh() :Component("Mesh Render")
{
	this->m_name = "Mesh_Comp";
}

Mesh::Mesh(const string& path) :Mesh()
{
    bool success = LoadModel(path);
    cout << "Load success " << success << " in path:"<<path << endl;

}

Mesh::Mesh(const string& path, Shader& _default_shader) :Mesh(path)
{	
	this->m_default_shader = _default_shader;
	bool success = LoadModel(path);

    // TODO: Add UI in LoadModel
    this->init_ui_content();
}

Mesh::~Mesh()
{
	//delete this->materials;
	for (int i = 0; i < this->materials.size();i++) {
		delete this->materials[i];	
	}
	this->materials.clear();
}

void Mesh::Render()
{
    for (unsigned int i = 0; i < this->m_Entries.size(); i++) {

        glBindVertexArray(this->m_Entries[i].VA);
        glBindBuffer(GL_ARRAY_BUFFER, this->m_Entries[i].VB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_Entries[i].IB);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        const unsigned int MaterialIndex = this->m_Entries[i].MaterialIndex;
        
        
        GameObject* gm = this->m_gameobject->cast_component<GameObject>(); // Get this gameobj's transform        
        this->materials[MaterialIndex]->set_model_matrix(gm->m_transform->m_model_matrix);        
		this->materials[MaterialIndex]->render();

        /*
        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }*/
        //glDrawElementsInstanced(GL_TRIANGLES, this->m_Entries[i].NumIndices, GL_UNSIGNED_INT,0 , 0);
        glDrawElements(GL_TRIANGLES, this->m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
        //glDrawElementsInstanced(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0, 0);
        
    }

    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);
}

void Mesh::Do()
{
	//cout << "Mesh Comp Do" << endl;
	this->Render();
}

Component* Mesh::copy()
{
    Mesh* _clone = new Mesh();
    _clone->m_name = this->m_name;
    _clone->m_Entries = this->m_Entries;
    _clone->m_default_shader = this->m_default_shader;
    _clone->materials = this->materials;

    _clone->m_srcPath = this->m_srcPath;
    _clone->m_srcDirecotory = this->m_srcDirecotory;

    return _clone;

}

void Mesh::init_ui_content()
{
    //this->create_panel(this->m_name);

    auto title_text = [&]() {Text("========= [ Mesh Render] ============");return true;};
    this->add_draw_item(title_text);

    for (auto _mat : this->materials) {
        //auto m_path_text = [&]() {Text(_mat->m_textures[Bind_Type::DIFFUSE]->get_file_path_char()); return true; };
        if (_mat->m_textures.size() > 0) {
            cout << _mat->m_textures[Bind_Type::DIFFUSE]->m_file_path.c_str() << endl;
            //TODO: Material UI
            //auto m_path_text = [=]() {Text(_mat->m_textures[Bind_Type::DIFFUSE]->m_file_path.c_str()); return true; };
            //this->add_draw_item(m_path_text);
        }
    }

    
}

bool Mesh::LoadModel(const string& path)
{
	// parse directory from path
	this->m_srcPath = path;
	const size_t last_slash_idx = path.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		this->m_srcDirecotory = path.substr(0, last_slash_idx);
		cout << "m_srcDirecotory " << this->m_srcDirecotory << endl;
	}


    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;
    Assimp::Importer Importer;

    
    const aiScene* pScene = Importer.ReadFile(path.c_str() , 
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);
      
    //const aiScene* pScene = Importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality );
    if (pScene) {        
        Ret = InitFromScene(pScene, path);
		InitMaterials(pScene);
    }
    else {
        printf("Error parsing '%s': '%s'\n", path.c_str(), Importer.GetErrorString());
    }



    return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    cout << "Loaded entries count " << m_Entries.size() << endl;
    //m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    // Scene是檔案下的空間，一個檔案可能包含多組mesh
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return true;
    //return InitMaterials(pScene, Filename);
}

bool Mesh::InitMaterials(const aiScene* pScene)
{
	if (pScene->HasMaterials())//True when number of materials is greater than 0
	{
		for (unsigned int m = 0; m < pScene->mNumMaterials; ++m)
		{

			aiMaterial* material = pScene->mMaterials[m];//Get the current material
			aiString materialName;//The name of the material found in mesh file
			aiReturn ret;//Code which says whether loading something has been successful of not

			ret = material->Get(AI_MATKEY_NAME, materialName);//Get the material name (pass by reference)
			if (ret != AI_SUCCESS) materialName = "";//Failed to find material name so makes var empty
						
			Material* mat = new Material(materialName.C_Str());
			mat->m_shader = &this->m_default_shader;


			//Diffuse maps
			int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);//Amount of diffuse textures
			aiString textureName;//Filename of the texture using the aiString assimp structure
            
			if (numTextures > 0)
			{
				//Get the file name of the texture by passing the variable by reference again
				//Second param is 0, which is the first diffuse texture
				//There can be more diffuse textures but for now we are only interested in the first one
				ret = material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureName);

				
				std::string textureFileName = textureName.data;//The actual name of the texture file
				cout << "init mat diff map " << textureFileName << endl;

				mat->add_texture(Bind_Type::DIFFUSE , this->m_srcDirecotory +'\\'+textureFileName);
			}            
            //Bump maps
            int numTextures_bump = material->GetTextureCount(aiTextureType_HEIGHT);//Amount of diffuse textures
            aiString textureName_bump;//Filename of the texture using the aiString assimp structure

            if (numTextures_bump > 0)
            {
                //Get the file name of the texture by passing the variable by reference again
                //Second param is 0, which is the first diffuse texture
                //There can be more diffuse textures but for now we are only interested in the first one
                ret = material->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), textureName_bump);


                std::string textureFileName = textureName_bump.data;//The actual name of the texture file
                cout << "init mat height map " << textureFileName << endl;

                mat->add_texture(Bind_Type::NORMAL , this->m_srcDirecotory + '\\' + textureFileName);
            }


            // Fininsh Loading this material
			this->materials.push_back(mat);
		}

		return true;
	}
	else
	{
		//TODO: Set diffuse color
		return false;
	}
}
/*
bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    return false;
}
*/


/*
 Load Mesh and create buffer for it.
*/
void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex; //TODO : mat

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos			= &(paiMesh->mVertices[i]);		
        const aiVector3D* pNormal		= paiMesh->HasNormals() ?  &(paiMesh->mNormals[i]) : &Zero3D;
        const aiVector3D* pTexCoord		= paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v(vec3(pPos->x, pPos->y, pPos->z),
            vec2(pTexCoord->x, pTexCoord->y),
            vec3(pNormal->x, pNormal->y, pNormal->z));

        Vertices.push_back(v);
    }
	cout << "text coord " << Vertices[0].m_uv.x << " " << Vertices[0].m_uv.y << endl;

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        //assert(Face.mNumIndices == 3);  //This usually works, but sometimes blocks the model for importing.
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }	

    cout << Index <<" mesh obj contains " << Vertices.size() << " verts " << Indices.size() << " index" << endl;

    m_Entries[Index].Init(Vertices, Indices);


}

void Mesh::Clear()
{
}

Mesh::MeshVert::MeshVert()
{
}

Mesh::MeshVert::~MeshVert()
{
}

void Mesh::MeshVert::Init(const vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();
    //cout << " mesh init " << NumIndices << endl;
    glGenVertexArrays(1, &this->VA);
    glBindVertexArray(this->VA);
    //glEnableVertexAttribArray(0);
    glGenBuffers(1, &this->VB);
    glBindBuffer(GL_ARRAY_BUFFER, this->VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
    
    //glEnableVertexAttribArray(1);
    glGenBuffers(1, &this->IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
    
    /*
    */
}
