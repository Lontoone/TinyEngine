#include "LightingManager.h"

void LightingManager::render_to_shadowmap()
{
	vector<Light*>* light_list = &Hierarchy::instance().m_ligths;
	for (auto light : *light_list)
	{
		//...
	}
}
