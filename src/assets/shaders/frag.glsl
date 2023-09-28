#version 330 core
out vec4 FragColor;
//in vec4 color;
in vec2 texcoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main(){
	//FragColor = vec4(1.0f , 0.25f , 0.574f ,1.0f);
	FragColor =  vec4(texcoord , 0,1);
	//FragColor = texture(tex1 , texcoord) * texture(tex2 , texcoord);
	//FragColor = texture(tex2 , texcoord) ;
}