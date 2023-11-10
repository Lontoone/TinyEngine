#version 330 core
uniform sampler2D screenTexture;
uniform vec2 u_mouse_position;
uniform vec2 u_texturesize;

in vec2 texcoord;
out vec4 color;
float u_magnifyRadius = 0.25;
float u_magnifyScale = 0.5;
vec2 u_magnifyCenter = vec2(0.5,0.5);

vec4 ring_color = vec4(1,0,0,1);

void main() {
        
    float asp = u_texturesize.x / u_texturesize.y;
    vec2 asp_v2 = vec2( 2.08,1);
    vec2 uv = texcoord ;
    
    // Calculate the distance from the magnify center
    float dist = distance(uv * asp_v2 , u_mouse_position * asp_v2);
    if (dist < u_magnifyRadius) {
        // Inside the magnify circle, scale the UV coordinates
        vec2 dir = uv - u_mouse_position;
        uv = u_mouse_position  + dir * u_magnifyScale;
    }
    //color = vec4(uv, 0,1);
    float ring_mask = 1- step( abs(length(texcoord * asp_v2 - u_mouse_position * asp_v2)- u_magnifyRadius), 0.001);
    // Sample the texture with the modified UV coordinates
    color = texture2D(screenTexture, uv) * ring_mask + ring_color* (1- ring_mask);
    /*
    */

}