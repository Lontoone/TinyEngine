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
struct AdditionalData{
    vec4 state;
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
layout(std430, binding = 3)buffer In_State {
    AdditionalData additional_state[];
};

//vec3 slight_pos = vec3(0,1,0);
uniform vec3 CAMERA_WORLD_POSITION;
uniform vec3 sun_postion;
uniform mat4 MATRIX_VP;
uniform mat4 view;
uniform mat4 projection;

out vec4 texcoord;
out vec3 light_pos;
out vec3 eye_dir;
out vec3 normal;

out  int draw_id;
out vec3 clip_pos;


void main()
{
    uint idx = draw_cmd[gl_DrawID].baseInstance + gl_InstanceID;
    uint cmd_idx = 0;
    vec4 obj_pos = (vertex + visiable_data[idx].offset);
    obj_pos.w = 1;

    /*
    vec4 P = view * vec4(obj_pos);
    vec3 T = aTangent.xyz; //normalize(mat3(view) * aTangent.xyz);
    vec3 N = normal.xyz ;//normalize(mat3(view) * normal.xyz);
    vec3 B = cross(N , T);
    vec3 L = light_pos - P.xyz;
    light_dir = normalize(vec3(dot(L,T) , dot(L,B) , dot(L,N) ));
    vec3 V = -P.xyz;
    eye_dir = normalize( vec3(dot(V, T ) , dot(V,B) , dot(V,N) ));
    */
    mat4 vp = projection * view;
    gl_Position = vp * obj_pos;
    //gl_Position = MATRIX_VP * obj_pos;
    clip_pos = gl_Position.xyz;
    eye_dir = normalize(CAMERA_WORLD_POSITION - clip_pos);
    
    light_pos = mat3(MATRIX_VP) * sun_postion;
    texcoord = aUv;
    draw_id = gl_DrawID;
    normal = aNormal.xyz;
}