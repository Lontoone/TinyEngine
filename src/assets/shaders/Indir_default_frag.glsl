#version 460 core

in vec3 clip_pos;
in vec3 normal;
in vec4 texcoord;
flat in int draw_id;
in vec3 light_dir;
in vec3 eye_dir;

out vec4 frag_color;

in vec3 light_pos ;

//uniform sampler2D DIFFUSE;
//uniform sampler2D NORMAL;
uniform sampler2DArray  albedoTextureArray; 
//uniform sampler2D  albedoTextureArray;

vec4 ka = vec4(0.1);
vec4 kd = vec4(0.7);
vec4 ks = vec4(0.5);
vec4 ambient = vec4(1,1,1,0);
void main() {

    // Diffuse 
    vec3 norm = normalize(normal);
    //vec3 lightDir = normalize(light_pos - clip_pos);
    vec3 lightDir = normalize(light_pos );
    float light_intensity = max(dot(norm, lightDir), 0.0) ;

    // Specular
    //float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(eye_dir, reflectDir), 0.0), 32);    

    /*
    vec3 V = normalize(eye_dir);
    vec3 L = normalize(light_dir);
    vec3 N = normalize(normal)*2.0 -vec3(1.0);
    float light_intensity = max(dot(N, L), 0.0);
    */

    //frag_color = texture(DIFFUSE, texcoord.xy) ;
    //frag_color = vec4(texcoord.xy,0,0);
    vec3 uv = vec3(texcoord.xy, draw_id);
    vec4 base_color = texture(albedoTextureArray, uv) *kd * light_intensity;
    //vec4 diffuse_color = vec4(1,1,1,0) * light_intensity * kd;
    vec4 sp_color = vec4(1,1,1,0) * spec * ks;

    //diffuse_color.xyz *= light_intensity;
    frag_color = ambient * ka  + base_color + sp_color ;
    
    //frag_color = texture(albedoTextureArray, texcoord.xy);
    if (frag_color.a <0.05) {
        discard;
    }
    /*
    */
}