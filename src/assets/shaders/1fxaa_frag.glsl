#version 410 core   
uniform sampler2D screenTexture;
//uniform bool enableFXAA;

const vec3 luma = vec3(0.299, 0.587, 0.114);

void main() {
    vec2 texcoord = gl_FragCoord.xy / textureSize(screenTexture, 0);
    vec3 color = texture(screenTexture, texcoord).rgb;
    // color=luma;

        float lumaNW = dot(texture(screenTexture, texcoord + vec2(-1.0, -1.0)/textureSize(screenTexture, 0)).rgb, luma);
        float lumaNE = dot(texture(screenTexture, texcoord + vec2(1.0, -1.0)/textureSize(screenTexture, 0)).rgb, luma);
        float lumaSW = dot(texture(screenTexture, texcoord + vec2(-1.0, 1.0)/textureSize(screenTexture, 0)).rgb, luma);
        float lumaSE = dot(texture(screenTexture, texcoord + vec2(1.0, 1.0)/textureSize(screenTexture, 0)).rgb, luma);
        float lumaM  = dot(color, luma);
        float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
        float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

        vec2 dir;
        dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
        dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

        float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * 0.5), 0.03);
        float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
        dir = min(vec2(0.5, 0.5), max(vec2(-0.5, -0.5), dir * rcpDirMin)) * 0.5;

        vec3 rgbA = 0.5 * (
            texture(screenTexture, texcoord.xy + dir * (1.0/3.0 - 0.5)).rgb +
            texture(screenTexture, texcoord.xy + dir * (2.0/3.0 - 0.5)).rgb);
        vec3 rgbB = rgbA * 0.5 + 0.25 * (
            texture(screenTexture, texcoord.xy + dir * -0.5).rgb +
            texture(screenTexture, texcoord.xy + dir * 0.5).rgb);

        float lumaB = dot(rgbB, luma);

        if ((lumaB < lumaMin) || (lumaB > lumaMax))
            color = rgbA;
        else
            color = rgbB;
    

    gl_FragColor = vec4(color, 0.0);

}