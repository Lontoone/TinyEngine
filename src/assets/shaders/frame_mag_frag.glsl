#version 330 core
uniform sampler2D screenTexture;

in vec2 texcoord;
out vec4 color;
float u_magnifyRadius = 0.25;
float u_magnifyScale = 0.5;
vec2 u_magnifyCenter = vec2(0.5,0.5);

vec4 ring_color = vec4(1,0,0,1);

void main() {
        
    vec2 uv = texcoord;
    // Calculate the distance from the magnify center
    float dist = distance(uv, u_magnifyCenter);

    if (dist < u_magnifyRadius) {
        // Inside the magnify circle, scale the UV coordinates
        vec2 dir = uv - u_magnifyCenter;
        uv = u_magnifyCenter + dir * u_magnifyScale;
    }

    float ring_mask = 1- step( abs(length(texcoord- u_magnifyCenter)- u_magnifyRadius), 0.001);
    // Sample the texture with the modified UV coordinates
    color = texture2D(screenTexture, uv) * ring_mask + ring_color* (1- ring_mask);


}