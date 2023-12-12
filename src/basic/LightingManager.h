#pragma once
#include <iostream>
#include <vector>
//#include <Hierarchy.h>
#include <Light.h>
#include <Mesh.h>
static class LightingManager
{
public:
	static void render_to_shadowmap();
	static void request_shadowmap_uniform(Shader& active_shader );

private:

};
