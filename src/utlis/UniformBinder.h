#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Fastsetup.h>
class UniformBinder
{
public:
	virtual void bind_uniform(unsigned int shader_pid) {};

private:

};
