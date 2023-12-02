#version 430 core
out vec4 FragColor;
in vec2 texcoord;

//in vec4 color;
in vec3 world_normal;
in mat3 TBN;
in vec3 light_dir;

layout(location = 0) out vec4 color_tex;
layout(location = 1) out vec4 color_normal;
layout(location = 2) out vec4 color_depth;


uniform sampler2D DIFFUSE;
uniform sampler2D NORMAL;
void main(){
	//FragColor = texture(NORMAL, texcoord);
	vec3 normal_map_color = texture(NORMAL, texcoord).rgb;
	vec3 tangentNormal = normalize(normal_map_color * 2.0 - 1.0);
	tangentNormal = normalize(TBN * tangentNormal);

	vec3 tangentLightDir = normalize(light_dir);
	float light =max( dot(tangentNormal, tangentLightDir) , 0) *0.5 +0.5; 
	//FragColor = vec4(light,0,0,1);
	FragColor =  texture(DIFFUSE , texcoord ) * light;
	//FragColor = vec4(normal_map_color,1.0);





	//color_tex = FragColor;
	color_tex = FragColor;
	//color_tex = vec4( normal_map_color , 1.0);
	color_normal = vec4(world_normal, 1.0);
	color_depth = vec4(vec3(gl_FragCoord.z), 1.0);

}