#version 410 core   
uniform sampler2D screenTexture; // The scene texture
uniform sampler2D texture2; // The scene texture
in vec2 texcoord;
out vec4 fragColor;

//uniform vec3 lightDir; // The direction of the light
vec3 lightDir = vec3(0.7,0.7,0);
void main() {    
    vec4 color = texture(screenTexture, texcoord);
    //fragColor = color;
    
   // Without a normal map, we can't calculate the normal here.
   // Instead, we'll just use a constant normal for simplicity.
   //vec3 normal = vec3(0.0, 0.0, 1.0);

   // Calculate the dot product of the normal and the light direction
   vec3 normal = texture(texture2, texcoord).xyz;
   float dotProduct = abs( dot(normal, lightDir));

   // Quantize the dot product into 3 levels to create the cel shading effect
   dotProduct = floor(dotProduct * 3.0) / 3.0;
   //fragColor = vec4(dotProduct,0,0, 1.0);

   // Multiply the color by the dot product to darken it
   color.rgb *= dotProduct;
   fragColor = vec4(color.rgb,  1.0);
   return;

   // Edge detection
   float edgeThreshold = 0.1; // Adjust this value to control the thickness of the edges
   vec2 offset = 1.0 / textureSize(screenTexture, 0);
   vec3 colorNorth = texture(screenTexture, texcoord + vec2(0.0, offset.y)).rgb;
   vec3 colorSouth = texture(screenTexture, texcoord - vec2(0.0, offset.y)).rgb;
   vec3 colorEast = texture(screenTexture, texcoord + vec2(offset.x, 0.0)).rgb;
   vec3 colorWest = texture(screenTexture, texcoord - vec2(offset.x, 0.0)).rgb;
   float edge = length(color.rgb - colorNorth) + length(color.rgb - colorSouth) + length(color.rgb - colorEast) + length(color.rgb - colorWest);
   if (edge > edgeThreshold) {
       color.rgb = vec3(0.0); // Set the edge color to black
   
   }

   fragColor = vec4(color.rgb, 0.0);
}