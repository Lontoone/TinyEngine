#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
//in vec2 texcoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main(){
	//FragColor = vec4(1.0f , 0.25f , 0.574f ,1.0f);
	FragColor = vec4(TexCoord,1.0f ,1.0f);
	//FragColor = texture(tex1 , texcoord) * texture(tex2 , texcoord);
	//FragColor = texture(tex2 , texcoord) ;
}