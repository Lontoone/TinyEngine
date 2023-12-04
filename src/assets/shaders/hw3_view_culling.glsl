#version 460 core

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

struct Offset
{
	vec4 position;
};
struct DrawCommand {
	uint count;
	uint instanceCount;
	uint firstIndex;
	uint baseVertex;
	uint baseInstance;	
};
struct AdditionalData {
	vec4 state;
};
layout(std430, binding = 0)buffer InPart_all {
	Offset all_pt[];
};

layout(std430, binding = 1)buffer InPart_target {
	Offset draw_pt[];
};
layout(std430, binding = 2)buffer In_cmd {
	DrawCommand draw_cmd[];
};
layout(std430, binding = 3)buffer In_State {
	AdditionalData additional_state[];
};

//uint data_count[3] = uint[3](0, 1010,2797);
uint data_count[3] = uint[3](0, 155304, 156314);
//uint data_count[3] = uint[3](0, 10000, 1510);
uniform mat4 MATRIX_M;
uniform mat4 MATRIX_VP;
uniform vec3 DOG_POS;
//uniform uint MAX_COUNT=10000;
void main() {

	// Global Unique index
	//uint idx = gl_GlobalInvocationID.x;
	uint idx = gl_GlobalInvocationID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y) + gl_GlobalInvocationID.y * gl_NumWorkGroups.x + gl_GlobalInvocationID.x;
	if (idx > 159111 || additional_state[idx].state.w <1) { return; }

	// get command index
	uint cmd_instance_idx = 0;

	if (idx <= data_count[1]) {
		cmd_instance_idx = 0;
	}
	else if (idx <= data_count[2]) {
		cmd_instance_idx = 1;
	}
	else {
		cmd_instance_idx = 2;
	}

	// Culling
	vec4 clip_pos = MATRIX_VP  * all_pt[idx].position*2 ;
	float clip_range = clip_pos.w;
	vec4 ndc_pos = clip_pos.xyzw / clip_pos.w;
	//vec4 clip_space_dog = MATRIX_VP * vec4( DOG_POS,1.0);

	if (distance(all_pt[idx].position.xyz, DOG_POS*2) <5) {   // I don't know why , but *2 works
		additional_state[idx].state.w = 0;
		return;
	}
	
	if (ndc_pos.x >1|| ndc_pos.y> 1 || ndc_pos.z >1 || ndc_pos.x <-1 || ndc_pos.y < -1 || ndc_pos.z < -1) {	
		return;
	}

	const uint unique_idx =  atomicAdd(draw_cmd[cmd_instance_idx].instanceCount, 1) + data_count[cmd_instance_idx];	
	draw_pt[unique_idx ] = all_pt[idx];


};
