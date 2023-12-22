#version 420 core
//layout(binding = 2)  uniform sampler2D texture1;//world pos;
layout(binding = 0) uniform sampler2D texture0;//color_ws_normal;
layout(binding = 1) uniform sampler2D texture1;//color_ws_pos;
layout(binding = 2) uniform sampler2D texture2;//color_ws_normal;
layout(binding = 3) uniform sampler2D texture3;//color_ws_pos;
layout(binding = 4) uniform sampler2D texture4;//color_ambient;
layout(binding = 5) uniform sampler2D texture5;//color_diffuse;
layout(binding = 6) uniform sampler2D texture6;//color_speculr;

layout(binding = 9)  uniform sampler2D u_TEX_SHADOW_MAP;
layout(binding = 10)  uniform samplerCube  u_TEX_SHADOW_MAP0;  // Point light
layout(binding = 11)  uniform sampler2D   u_TEX_LTC_MAP0;  // area light ltc1
layout(binding = 12)  uniform sampler2D   u_TEX_LTC_MAP1;  // area light ltc2

uniform vec3 u_CAM_POS;
uniform mat4 u_PROJ_MATRIX;
uniform mat4 view;
uniform mat4 inv_view;
//===================================
//			Light Prop
//===================================
uniform mat4 u_LIGHT_VP_MATRIX;
uniform mat4 u_AREA_LIGHT_Model_MAT;
uniform vec4 u_LIGHT_WORLD_POS0;
uniform vec4 u_LIGHT_WORLD_POS1;
uniform float u_POINTLIGHT1_FAR;
uniform bool u_USE_VOLUMN_LIGHT;

uniform int SCR_WIDTH ;
uniform int SCR_HEIGHT;

uniform vec3 al_points[4];

const float LUT_SIZE = 64.0; // ltc_texture size
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS = 0.5 / LUT_SIZE ;

//out vec4 fragColor;
in vec2 texcoord;
out vec4 color;

float ia = 0.1; //ToDo: ambient term
float id = 0.7;
float is = 0.7;

struct Light {

	float constant;
	float linear ;
	float quadratic ;
};
Light pl1_para;


//===================================
//		Area Light Functions
//===================================
// Vector form without project to the plane (dot with the normal)
// Use for proxy sphere clipping
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2)
{
	// Using built-in acos() function will result flaws
	// Using fitting result for calculating acos()
	float x = dot(v1, v2);
	float y = abs(x);

	float a = 0.8543985 + (0.4965155 + 0.0145206 * y) * y;
	float b = 3.4175940 + (4.1616724 + y) * y;
	float v = a / b;

	float theta_sintheta = (x > 0.0) ? v : 0.5 * inversesqrt(max(1.0 - x * x, 1e-7)) - v;

	return cross(v1, v2) * theta_sintheta;
}

float IntegrateEdge(vec3 v1, vec3 v2)
{
	return IntegrateEdgeVec(v1, v2).z;
}
// P is fragPos in world space (LTC distribution)
vec3 LTC_Evaluate(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 points[4], bool twoSided)
{
	// construct orthonormal basis around N
	vec3 T1, T2;
	T1 = normalize(V - N * dot(V, N));
	T2 = cross(N, T1);

	// rotate area light in (T1, T2, N) basis
	Minv = Minv * transpose(mat3(T1, T2, N));

	// polygon (allocate 4 vertices for clipping)
	vec3 L[4];
	// transform polygon from LTC back to origin Do (cosine weighted)
	L[0] = Minv * (points[0] - P);
	L[1] = Minv * (points[1] - P);
	L[2] = Minv * (points[2] - P);
	L[3] = Minv * (points[3] - P);

	// use tabulated horizon-clipped sphere
	// check if the shading point is behind the light
	vec3 dir = points[0] - P; // LTC space
	vec3 lightNormal = cross(points[1] - points[0], points[3] - points[0]);
	bool behind = (dot(dir, lightNormal) < 0);

	// cos weighted space
	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);

	// integrate
	vec3 vsum = vec3(0.0);
	vsum += IntegrateEdgeVec(L[0], L[1]);
	vsum += IntegrateEdgeVec(L[1], L[2]);
	vsum += IntegrateEdgeVec(L[2], L[3]);
	vsum += IntegrateEdgeVec(L[3], L[0]);

	// form factor of the polygon in direction vsum
	float len = length(vsum);

	float z = vsum.z / len;
	/*
	*/
	if (behind)
		z = -z;

	vec2 uv = vec2(z * 0.5f + 0.5f, len); // range [0, 1]
	uv = uv * LUT_SCALE + LUT_BIAS;

	// Fetch the form factor for horizon clipping
	float scale = texture(u_TEX_LTC_MAP1, uv).w;

	float sum = len * scale;
	if (!behind && !twoSided)
		sum = 0.0;

	// Outgoing radiance (solid angle) for the entire polygon
	vec3 Lo_i = vec3(sum, sum, sum);
	return Lo_i;
}

// PBR-maps for roughness (and metallic) are usually stored in non-linear
// color space (sRGB), so we use these functions to convert into linear RGB.
vec3 PowVec3(vec3 v, float p)
{
	return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float gamma = 2.2;
vec3 ToLinear(vec3 v) { return PowVec3(v, gamma); }
vec3 ToSRGB(vec3 v) { return PowVec3(v, 1.0 / gamma); }


// Volumetric Light
/*
float G_SCATTERING = 0.5;
float ComputeScattering(float lightDotView)
{
	float result = 1.0f - G_SCATTERING * G_SCATTERING;
	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * lightDotView, 1.5f));
	return result;
}
*/

uniform bool u_USE_SHADOW;   //Not using....

void main() {
	pl1_para.constant = 1.0;
	pl1_para.linear = 0.7;
	pl1_para.quadratic = 0.01;

	float vl_sum = 0;  //volumn light counter

	//============================================
	//                 Read Texture
	//============================================
	vec4 diffuse_color = texture(texture5, texcoord);
	vec4 world_normal = texture(texture2, texcoord);
	vec4 ambient_color = texture(texture4, texcoord);
	vec4 specular_color = texture(texture6, texcoord);  // rgb = color tint , a = shininess
	//============================================

	//==============================================
	//			Main Light Shadow
	//==============================================
	vec2 scr_pix = vec2(texcoord.x * SCR_WIDTH, texcoord.y * SCR_HEIGHT);
	vec4 world_pos = texelFetch(texture3,ivec2(scr_pix.xy) ,0); // model * aPos

	vec4 light_clip_pos = u_LIGHT_VP_MATRIX * world_pos;
	vec3 light_ndc_pos = light_clip_pos.xyz / light_clip_pos.w;

	vec4 shadow_color = texture(u_TEX_SHADOW_MAP, light_ndc_pos.xy);
	float shadow_term = 0;
	if (light_ndc_pos.z < shadow_color.r ) {
		shadow_term = 1;
	}
	else {		
		shadow_term = 0;
	}
	//color = vec4(shadow_term,0,0,1);
	//return;
	//==============================================
	//			Point Light 1
	//==============================================
	
	vec3 light_dir = (world_pos.xyz - u_LIGHT_WORLD_POS1.xyz);  //todo: point light position	
	float closestDepth = texture(u_TEX_SHADOW_MAP0, light_dir).r;
	closestDepth *= u_POINTLIGHT1_FAR; 
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(light_dir);
	// now test for shadows
	float bias = -0.050f;

	float attenuation = 1.0 / (pl1_para.constant + pl1_para.linear * currentDepth  +
		pl1_para.quadratic * (currentDepth * currentDepth ));	
	float pl1_shadow = currentDepth > closestDepth - bias ? attenuation : 0;
	float point_light1 = (1 - pl1_shadow) * attenuation;

	//==============================================
	//			Area Light
	//==============================================
	vec3 ar_N = normalize(world_normal.xyz);
	vec3 ar_V = normalize(u_CAM_POS.xyz - world_pos.xyz);   //normalize(viewPosition - worldPosition);
	vec3 ar_P = world_pos.xyz;
	float dotNV = clamp(dot(ar_N, ar_V), 0.0f, 1.0f);

	// use roughness and sqrt(1-cos_theta) to sample M_texture
	//vec2 ar_uv = vec2(material.albedoRoughness.w, sqrt(1.0f - dotNV));
	vec2 ar_uv = vec2(specular_color.a, sqrt(1-dotNV));
	ar_uv = ar_uv * LUT_SCALE + LUT_BIAS;

	// get 4 parameters for inverse_M
	vec4 t1 = texture(u_TEX_LTC_MAP0, ar_uv);
	// Get 2 parameters for Fresnel calculation
	vec4 t2 = texture(u_TEX_LTC_MAP1, ar_uv);

	mat3 Minv = mat3(
		vec3(t1.x, 0, t1.y),
		vec3(0, 1, 0),
		vec3(t1.z, 0, t1.w)
	);

	// translate light source for testing
	vec3 translatedPoints[4];
	vec3 offset = vec3(0,0,0);
	translatedPoints[0] = (u_AREA_LIGHT_Model_MAT * vec4( al_points[0],1.0)).xyz;
	translatedPoints[1] = (u_AREA_LIGHT_Model_MAT * vec4( al_points[1],1.0)).xyz;
	translatedPoints[2] = (u_AREA_LIGHT_Model_MAT * vec4( al_points[2],1.0)).xyz;
	translatedPoints[3] = (u_AREA_LIGHT_Model_MAT * vec4( al_points[3],1.0)).xyz;
	/*
	translatedPoints[0] = (u_AREA_LIGHT_Model_MAT * vec4(-1, 1, -1, 1)).xyz + offset;
	translatedPoints[1] = (u_AREA_LIGHT_Model_MAT * vec4(-1, 1, 1, 1)).xyz + offset;
	translatedPoints[2] = (u_AREA_LIGHT_Model_MAT * vec4(-1, 0, 1, 1)).xyz + offset;
	translatedPoints[3] = (u_AREA_LIGHT_Model_MAT *vec4(-1, 0, -1,1)).xyz+offset ;
	*/

	// Evaluate LTC shading
	vec3 ar_diffuse = LTC_Evaluate(ar_N, ar_V, ar_P, mat3(1), translatedPoints, false);
	vec3 specular = LTC_Evaluate(ar_N, ar_V, ar_P, Minv, translatedPoints, false);

	// GGX BRDF shadowing and Fresnel
	// t2.x: shadowedF90 (F90 normally it should be 1.0)
	// t2.y: Smith function for Geometric Attenuation Term, it is dot(V or L, H).	
	specular *= is * t2.x + (1.0f - is) * t2.y;

	//result = areaLight.color * areaLight.intensity * (specular + mDiffuse * diffuse);
	//vec3 result = vec3(0.8, 0.6, 0.0) * (specular + id * ar_diffuse);
	vec3 result = vec3(0.8, 0.6, 0.0) * (ar_diffuse);

	vec4 area_light = vec4(ToSRGB(result), 1.0f);
	//color = vec4(ar_diffuse, 1.0);
	//color = area_light;
	//return;


	//==============================================
	//			Diffuse
	//==============================================
	vec3 world_light_dir = normalize(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz);
	vec4 diffuse = texture(texture5, texcoord);
	float d_intensity = max(dot(world_normal.xyz, world_light_dir), 0.0f);

	//==============================================
	//			Specular
	//==============================================
	//vec3 world_view_dir = normalize(u_CAM_POS - world_pos.xyz);
	vec3 view_normal = mat3(view) * world_normal.xyz;  //N
	vec3 view_pos = mat3(view) * (u_CAM_POS - world_pos.xyz);					//P
	vec3 L = mat3(view) *(u_LIGHT_WORLD_POS0.xyz - world_pos.xyz);  // L
	vec3 V = -view_pos.xyz;  //V

	L = normalize(L);
	V = normalize(V);
	vec3 R = reflect(-L , view_normal);
	view_pos = normalize(view_pos);
	float specular_term = pow(max(dot(R, V), 0.0f), specular_color.a);

	//==============================================
	//			Volumetric Light
	//==============================================
	float total_intensity = 0;
	if (u_USE_VOLUMN_LIGHT) {
		vec4 vl_start_pos = vec4(0, 0, 1, 1); // in view space	
		vec3 vl_move_dir = normalize(vec3(texcoord, 1.0) * 2 - 1);
		vl_move_dir = mat3(view) * vl_move_dir;


		int max_steps = 100;
		float traveled_distance = 0;
		float step_size = 0.05f;

		for (int i = 0; i < max_steps; i++) {
			//vl_sum += 0.25;
			vl_start_pos.xyz += vl_move_dir * step_size;
			traveled_distance += step_size;

			vec4 mvoing_world_pos = inv_view * vl_start_pos;
			vec4 vl_light_pos = u_LIGHT_VP_MATRIX * mvoing_world_pos;

			vec3 vl_proj_uv = (vl_light_pos.xyz / vl_light_pos.w).xyz;
			vec4 shadow_color = texture(u_TEX_SHADOW_MAP, vl_proj_uv.xy);
			color = vec4(vl_proj_uv.xy, 0, 1);
			//return;


			if (vl_proj_uv.z < shadow_color.r && length(mvoing_world_pos.xyz - u_CAM_POS.xyz) < length(world_pos.xyz - u_CAM_POS.xyz)) {
				//total_intensity += ((i/ max_steps) * decay);
				//total_intensity += ( 0.5 + (pow(i/100 ,2)));
				total_intensity += 0.01 + 1 / i / max_steps;
			}
			else {
				//break;
			}

		}
	}
	else {
		total_intensity = 0;
	}


	//color = total_intensity * diffuse;
	//color = vec4(total_intensity , 0 , 0 , 1.0) * diffuse + shadow_term;
	//color = total_intensity ;
	
	//return;
	/*
	*/
	/*
	int NUM_SAMPLES = 100;
	float Density = 0.926;
	float Weight = 0.0587;
	float Decay = 0.56;
	float Exposure = 0.2;

	vec2 moving_uv = texcoord;
	// Calculate vector from pixel to light source in screen space.    
	vec2 deltaTexCoord = (texcoord - (u_PROJ_MATRIX * mat4(view) * (u_LIGHT_WORLD_POS0)).xy);
	// Divide by number of samples and scale by control factor.   
	deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;
	// Store initial sample.    
	vec4 vl_bg_color = texture(texture5, moving_uv);
	// Set up illumination decay factor.    
	float illuminationDecay = 1.0f;
	// Evaluate summation from Equation 3 NUM_SAMPLES iterations.    
	for (int i = 0; i < NUM_SAMPLES; i++) {
		// Step sample location along ray.     
		moving_uv -= deltaTexCoord;
		// Retrieve sample at new location.    
		vec4 vl_diffuse= texture(texture5, moving_uv); // diffuse color
		// Apply sample attenuation scale/decay factors.     
		vl_diffuse *= illuminationDecay * Weight;
		// Accumulate combined color.     
		vl_bg_color += vl_diffuse;
		// Update exponential decay factor.     
		illuminationDecay *= Decay;
	}
	// Output final color with a further scale control factor.    
	color = vl_bg_color* Exposure + diffuse ;
	return;
	*/


	//color = (ambient_color * ia + vec4(1) * d_intensity + specular_color * specular_term) * diffuse * shadow_term; 
	color = ambient_color * ia * diffuse +
		diffuse * d_intensity * shadow_term * id +
		specular_color * specular_term * shadow_term * is * diffuse +
		point_light1 +
		area_light +
		total_intensity * diffuse 
		;



}