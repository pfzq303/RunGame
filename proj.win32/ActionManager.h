#pragma once
#include "MHeader.h"
class ActionManager :
	public CCNode
{
public:
	ActionManager(void);
	~ActionManager(void);

	static CCAction* getRoleAction(ROLE_STATE);
};

