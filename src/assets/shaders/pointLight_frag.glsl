#version 330 core
in vec4 FragPos;

uniform vec4 u_LIGHT_WORLD_POS1;
//uniform float far_plane;
//float far_plane = 10;
uniform float u_POINTLIGHT1_FAR;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - u_LIGHT_WORLD_POS1.xyz);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_POINTLIGHT1_FAR; 

    // write this as modified depth
    gl_FragDepth = lightDistance;
}