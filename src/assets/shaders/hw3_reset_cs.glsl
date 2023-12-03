#version 430 core

layout(local_size_x = 3, local_size_y = 1, local_size_z = 1) in;

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

uniform float time = 0;
uniform uint MAX_COUNT = 159111;
void main() {

	uint idx = gl_LocalInvocationID.x;
	//uint cmd_instance_idx = int( floor(idx/ MAX_COUNT));
	if (idx > MAX_COUNT) {return;}

	//draw_cmd[cmd_instance_idx].instanceCount = int(floor(time*10)) ;
	//draw_cmd[cmd_instance_idx].instanceCount = 0;
	//draw_pt[idx].position = all_pt[idx].position + sin(time);
	draw_cmd[idx].instanceCount = 0;
	//draw_pt[idx].position = all_pt[idx].position;
	//draw_pt[idx].position.x += sin(idx + time);


	//pt[idx].position = pt[idx].position + sin(time) * 0.01f;
	//pt[idx].position = vec4( sin(time) , sin(time) , sin(time),1)*10;
};