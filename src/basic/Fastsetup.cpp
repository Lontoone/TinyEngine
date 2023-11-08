
#include "Fastsetup.h"
#pragma warning (disable : 4996)
char* textFileRead(const char* fn) {	

	FILE* fp;
	char* content = NULL;

	int count = 0;

	if (fn != NULL) {
		fp = fopen(fn, "rt");
		//errno_t error = fopen_s(&fp,fn, "rt");

		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {
			printf("The file \"%s\" was not opened\n", fn);
		}
	}
	return content;
}


//void SetProgram(MechainState& mechainState , const char* vs_file , const char* fs_file)
void SetProgram(MechainState& mechainState, const string vs_file, const string fs_file)
{
	GLuint v, f, p;
	char* vs = NULL;
	char* fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead(vs_file.c_str());
	fs = textFileRead(fs_file.c_str());

	glShaderSource(v, 1, (const GLchar**)&vs, NULL);
	glShaderSource(f, 1, (const GLchar**)&fs, NULL);

	free(vs);
	free(fs);

	GLint success;
	char infoLog[1000];
	// compile vertex shader
	glCompileShader(v);
	// check for shader compile errors
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(v, 1000, NULL, infoLog);
		std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}

	// compile fragment shader
	glCompileShader(f);
	// check for shader compile errors
	glGetShaderiv(f, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(f, 1000, NULL, infoLog);
		std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}

	// create program object
	p = glCreateProgram();

	// attach shaders to program object
	glAttachShader(p, f);
	glAttachShader(p, v);

	// link program
	glLinkProgram(p);
	// check for linking errors
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(p, 1000, NULL, infoLog);
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(v);
	glDeleteShader(f);

	if (success) {
		glUseProgram(p);
	}
	else
	{
		system("pause");
		exit(123);
	}

	mechainState.fragShaderId	= f;
	mechainState.vertShaderId	= v;
	mechainState.programId		= p;
	
}

Shader::Shader()
{
}

//Shader::Shader(const string src_path,const string _shaderName)
Shader::Shader(const string vert_path, const string frag_path)
{
	this->m_state = MechainState();
	/*
	SetProgram(this->m_state,
		src_path +"\\"+ _shaderName + string("_vert.glsl"),
		src_path +"\\"+ _shaderName + string("_frag.glsl"));
	*/
	SetProgram(this->m_state,
		vert_path,
		frag_path);

	this->init_variables();
}

Shader::~Shader()
{
	//delete &this->m_state;
}

void Shader::init_variables()
{
	for (auto &pair : this->shader_variables) {
		pair.second = glGetUniformLocation(this->m_state.programId, pair.first);
	}
}

void Shader::activate()
{
	glUseProgram(this->m_state.programId);
}

