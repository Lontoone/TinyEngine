#version 460 core
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
struct DrawCommand {
    uint count;
    uint instanceCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
};
layout(std430, binding = 1) buffer visiable_offset {
    OffsetData visiable_data[];
};
layout(std430, binding = 2)buffer In_cmd {
    DrawCommand draw_cmd[];
};

uniform mat4 MATRIX_VP;
uniform mat4 view;

out vec4 texcoord;
out  int draw_id;

void main()
{
    
    uint idx = draw_cmd[gl_DrawID].baseInstance + gl_InstanceID;
    uint cmd_idx = 0;
    
    gl_Position = MATRIX_VP * (vertex + visiable_data[idx].offset );
    
    texcoord = aUv;
    draw_id = gl_DrawID;
}