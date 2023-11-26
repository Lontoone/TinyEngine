#version 330 core
uniform sampler2D screenTexture;
uniform mat4 MATRIX_VP;

in vec2 texcoord;
out vec4 frag_outcolor;

uniform float u_grid_count = 1000; // grid ratio
bool gridTextureGradBox( vec4 p)
{
    // TODO: AA filter...
    return (fract(p.x * u_grid_count) < 0.01f || fract(p.z * u_grid_count) < 0.01f);
    
}
    

void main() {

    bool is_grid = gridTextureGradBox( vec4(texcoord.x , 0, texcoord.y,1));
    if (!is_grid) {
        discard;
    }
    frag_outcolor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
	//color = max( vec4(0) , vec4(grid, 0, 0, 1));
	//color = vec4(texcoord,0,1);
}
