#version 430 core

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

uniform mat4 MATRIX_VP;
uniform uint MAX_COUNT=10000;
void main() {

	uint idx = gl_GlobalInvocationID.x;
	uint cmd_instance_idx = int(floor(idx / MAX_COUNT));

	vec4 world_position = MATRIX_VP * all_pt[idx].position ;
	float clip_range = world_position.w ;
	//vec4 ndc_pos = world_position.xyzw / world_position.w;

	/**/
	if (idx > MAX_COUNT) {
		return;
	}

	//if (ndc_pos.x >1 || ndc_pos.y> 1 || ndc_pos.z >1 || ndc_pos.x <-1 || ndc_pos.y < -1 || ndc_pos.z < -1) {
	if (world_position.x > clip_range || world_position.y > clip_range*1.25 || world_position.z > clip_range || world_position.x < -clip_range || world_position.y < -clip_range*1.25 || world_position.z < -clip_range) {
		return;
	}
	draw_pt[atomicAdd(draw_cmd[cmd_instance_idx].instanceCount, 1)] = all_pt[idx];   //draw cmd maybe bug

	//draw_pt[idx].position = all_pt[idx].position + sin(time);

};