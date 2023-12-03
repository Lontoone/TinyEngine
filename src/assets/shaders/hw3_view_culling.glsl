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

layout(std430, binding = 0)buffer InPart_all {
	Offset all_pt[];
};

layout(std430, binding = 1)buffer InPart_target {
	Offset draw_pt[];
};
layout(std430, binding = 2)buffer In_cmd {
	DrawCommand draw_cmd[];
};

//uint data_count[3] = uint[3](0, 1010,2797);
uint data_count[3] = uint[3](0, 155304, 156314);
//uint data_count[3] = uint[3](0, 10000, 1510);
uniform mat4 MATRIX_VP;
//uniform uint MAX_COUNT=10000;
void main() {

	// Global Unique index
	//uint idx = gl_GlobalInvocationID.x;
	uint idx = gl_GlobalInvocationID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y) + gl_GlobalInvocationID.y * gl_NumWorkGroups.x + gl_GlobalInvocationID.x;
	if (idx > 159111) { return; }

	// get command index
	uint cmd_instance_idx = 0;
	/*
	if (idx >= draw_cmd[1].baseInstance) {
		cmd_instance_idx = 1; 
	}
	*/
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
	vec4 world_position = MATRIX_VP * all_pt[idx].position ;
	float clip_range = world_position.w ;
	//vec4 ndc_pos = world_position.xyzw / world_position.w;

	
	/*
	if (idx > 3819) {
		return;
	}
	*/
	
	//if (ndc_pos.x >1 || ndc_pos.y> 1 || ndc_pos.z >1 || ndc_pos.x <-1 || ndc_pos.y < -1 || ndc_pos.z < -1) {
	if (world_position.x > clip_range || world_position.y > clip_range*1.25 || world_position.z > clip_range || world_position.x < -clip_range || world_position.y < -clip_range*1.25 || world_position.z < -clip_range) {
		return;
	}

	const uint unique_idx =  atomicAdd(draw_cmd[cmd_instance_idx].instanceCount, 1) + data_count[cmd_instance_idx];
	//draw_pt[atomicAdd(draw_cmd[cmd_instance_idx].instanceCount, 1)] = all_pt[idx];   //draw cmd maybe bug
	draw_pt[unique_idx ] = all_pt[idx];

	//uint _pt_idx = unique_idx + draw_cmd[cmd_instance_idx].instanceCount;
	//draw_pt[unique_idx].position = all_pt[idx].position;//+ sin(time);

};
