
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

int compile_shader(unsigned int& p , unsigned int& o_id ) {

	GLint success;
	char infoLog[1000];	
	glCompileShader(o_id);
	// check for shader compile errors
	glGetShaderiv(o_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(o_id, 1000, NULL, infoLog);
		std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	}
	glAttachShader(p, o_id);	
	glLinkProgram(p);
	
	glGetProgramiv(p, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(p, 1000, NULL, infoLog);
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(o_id);

	return success;
}

//void SetProgram(MechainState& mechainState , const char* vs_file , const char* fs_file)
void SetProgramFromSource(MechainState& mechainState, const char* vs, const char* fs , bool create_one ) {

	GLuint v, f, p;
	if (create_one) {
		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);		
		p = glCreateProgram();

		mechainState.fragShaderId = f;
		mechainState.vertShaderId = v;
		mechainState.programId = p;
	}
	else
	{
		v = mechainState.vertShaderId;
		f = mechainState.fragShaderId;
		p = mechainState.programId;

	}

	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);

	GLint success = 1;
	success *= compile_shader(p,v);
	success *= compile_shader(p,f);
	
	if (success) {
		glUseProgram(p);
	}
	else
	{
		system("pause");
		exit(123);
	}
}



void UpdateFromSource(const MechainState& mechainState, const char* v_text, const char* fs_char)
{



}

/* Create Computer shader */
void SetProgram(MechainState& mechainState , const string vs_file) {
	mechainState.vertShaderId = glCreateShader(GL_COMPUTE_SHADER);
	mechainState.programId = glCreateProgram();

	char* vs = NULL;
	vs = textFileRead(vs_file.c_str());

	glShaderSource(mechainState.vertShaderId, 1, (const GLchar**)&vs, NULL);
	GLint success = 1;
	success *= compile_shader(mechainState.programId, mechainState.vertShaderId);
	
	if (success)
		glUseProgram(mechainState.programId);
	else
	{
		system("pause");
		exit(123);
	}
}
void SetProgram(MechainState& mechainState, const string vs_file, const string fs_file)
{
	char* vs = NULL;
	char* fs = NULL;

	// create program object
	//SetProgramFromSource(mechainState , vs,fs);
	GLuint v, f, p;
	GLint success =1;

	p = glCreateProgram();
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);


	if (!vs_file.empty()) {
		vs = textFileRead(vs_file.c_str());
		glShaderSource(v, 1, (const GLchar**)&vs, NULL);
		success *= compile_shader(p, v);	
		free(vs);
	}
	if (!fs_file.empty()) {
		fs = textFileRead(fs_file.c_str());
		glShaderSource(f, 1, (const GLchar**)&fs, NULL);
		success *= compile_shader(p, f);
		free(fs);
	}

	if (success) {
		glUseProgram(p);
	}
	else
	{
		system("pause");
		exit(123);
	}

	mechainState.fragShaderId = f;
	mechainState.vertShaderId = v;
	mechainState.programId = p;


}

Shader::Shader()
{
	this->m_state = MechainState();
}

//Shader::Shader(const string src_path,const string _shaderName)
Shader::Shader(const string vert_path, const string frag_path)
{
	this->m_state = MechainState();
	SetProgram(this->m_state,
		vert_path,
		frag_path);

	this->init_variables();
}


Shader::Shader(const string vert_path)
{
	// Load Compute Shader
	this->m_state = MechainState();
	SetProgram(this->m_state,vert_path);	
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
void Shader::add_variables(const char* name)
{
	this->shader_variables.insert(std::pair<const char* , GLuint>( name, glGetUniformLocation(this->m_state.programId , name)));
}
void Shader::add_variables(const string name)
{
	this->shader_variables.insert(std::pair<const char*, GLuint>(name.c_str(), glGetUniformLocation(this->m_state.programId, name.c_str())));
}
void Shader::activate()
{
	glUseProgram(this->m_state.programId);
}

