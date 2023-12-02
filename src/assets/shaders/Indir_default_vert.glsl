#version 430 core
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 aUv;
layout(location = 2) in vec4 aNormal;
layout(location = 3) in vec4 aTangent;

/**/
//layout(location = 10) in vec4 offset;
//layout(location = 1) in vec4 color;
struct OffsetData {
    vec4 offset;
};
layout(std430, binding = 1) buffer visiable_offset {
    OffsetData visiable_data[];
};

uniform mat4 MATRIX_VP;
out vec4 texcoord;

void main()
{
    //gl_Position = MATRIX_VP * vec4(position.xy + gl_InstanceID*0.1, 0, 1.0);
    //gl_Position = MATRIX_VP * (vertex + offset);
    //gl_Position = MATRIX_VP * (vertex );
    gl_Position = MATRIX_VP * (vertex + visiable_data[gl_InstanceID].offset);
    
    texcoord = aUv;
}