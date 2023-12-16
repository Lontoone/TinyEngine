#version 330
//out vec4 color;
out vec4 fragColor;
void main()
{
	// Keep it empty
	//color = vec4(0.5);
	// Opengl Depth = gl_FragCoord.z
	fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
