#pragma once
#include "MHeader.h"
class ActionManager :
	public CCNode
{
public:
	ActionManager(void);
	~ActionManager(void);
	CREATE_FUNC(ActionManager);
	CCAction* getRoleAction(ROLE_STATE,ROLE_STATE);
};

