#version 430 core
out vec4 FragColor;
in vec2 texcoord;

//in vec4 color;

in vec3 world_normal;
in mat3 TBN;
in vec3 light_dir;
in vec4 world_clip_pos;
in vec4 light_clip_pos;

in vec3 world_light_dir;
in vec3 world_view_dir;
in vec3 world_half_dir;

layout(location = 0) out vec4 color_tex;
layout(location = 1) out vec4 color_normal;
layout(location = 2) out vec4 color_depth;


uniform sampler2D DIFFUSE;
uniform sampler2D NORMAL;
uniform sampler2D u_TEX_SHADOW_MAP;
// Gets the position of the camera from the main function
uniform vec3 u_CAM_POS;

vec3 lightPos = vec3(0, 1, 0);
float specAmount = 0.5;
vec4 specularLight = vec4(1,1,1,1);

float near = 1;
float far = 50;
float linear_depth(float d) {
	d = (2 * near * far) / (far + near - (d * 2.0 - 1.0) * (far - near));
	return d / far;
}
vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;
	// =====================================
	//			Diffuse lighting
	// =====================================
	vec3 normal = normalize(world_normal);
	vec3 lightDirection = normalize(lightPos);
	// Diffuse light
	float diffuse = max(dot(world_normal, world_light_dir), 0.0f);	
	//float diffuse = 1;  // Some model have no normal vector


	// =====================================
	//			Specular lighting
	// =====================================	
	float specular = 0.0f;	
	float specularLight = 0.50f;	
	
	float specAmount = pow(max(dot(normal, world_view_dir), 0.0f), 16);
	specular = specAmount * specularLight;
	//return vec4(specular,0,0, 1);
	//return vec4(world_view_dir, 1);


	// Shadow value
	float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 light_ndc_pos = light_clip_pos.xyz / light_clip_pos.w;
	float currentDepth = light_clip_pos.z;
	float closestDepth = texture(u_TEX_SHADOW_MAP, light_ndc_pos.xy).r;
	if (currentDepth > closestDepth)
		shadow = 1;
	/*
	if (lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(u_TEX_SHADOW_MAP, 0);
		for (int y = -sampleRadius; y <= sampleRadius; y++)
		{
			for (int x = -sampleRadius; x <= sampleRadius; x++)
			{
				float closestDepth = texture(u_TEX_SHADOW_MAP, lightCoords.xy + vec2(x, y) * pixelSize).r;				
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;
			}
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}*/


	float lin_shadow = linear_depth(texture(u_TEX_SHADOW_MAP, light_ndc_pos.xy).r);
	float lin_dep = linear_depth(currentDepth);
	//return vec4(world_normal,1.0);
	return texture(DIFFUSE, texcoord) * (diffuse * (1.0f - shadow) + ambient)
			+  specular * (1.0f - shadow);
			//+ texture(specular0, texCoord).r * specular * (1.0f - shadow)) * lightColor;
}


void main(){

	FragColor = direcLight();
	//color_tex = FragColor;
	color_tex = FragColor;
	//color_tex = vec4(light , 0,0,1);
	//color_tex = vec4( normal_map_color , 1.0);
	color_normal = vec4(world_normal, 1.0);
	color_depth = vec4(vec3(gl_FragCoord.z), 1.0);


}