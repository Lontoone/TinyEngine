#pragma once
#ifndef COMPONENT
#define COMPONENT

class Component
{
public:
	Component();
	int m_execute_order;
	const char* m_name;
	virtual void Do();

};

#endif // !COMPONENT
