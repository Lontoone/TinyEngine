#version 330

layout (location = 0) in vec3 Position;

//uniform mat4 gWVP;
uniform mat4 model;
uniform mat4 u_LIGHT_VP_MATRIX;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    gl_Position = u_LIGHT_VP_MATRIX * model * vec4(Position, 1.0);
}
