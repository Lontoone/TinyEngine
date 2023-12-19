#pragma once
#ifndef UIAVLE_COMPONENT
#define UIAVLE_COMPONENT

#include <Component.h>
#include <UiPanel.h>

class UiableComponent : virtual public UiPanel, virtual public Component
{
public :
	bool m_activate = true;
	virtual void init_ui_content() {};

};

#endif // !UIAVLE_COMPONENT