#pragma once
#ifndef UIAVLE_COMPONENT
#define UIAVLE_COMPONENT

#include <Component.h>
#include <UiPanel.h>

class UiableComponent : virtual public UiPanel, virtual public Component
{
public :
	virtual void init_ui_content() {};

};

#endif // !UIAVLE_COMPONENT