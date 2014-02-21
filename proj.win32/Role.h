#pragma once
#include "MHeader.h"
#include "ActionManager.h"

class Role :
	public CCNode
{
public:
	Role(void);
	~Role(void);

	virtual bool init();
	void setRolePosition(CCPoint);
	CCRect getNextRect();
	void roleGo();
	void roleGo(float,float);
	float getXRepairDistance(CCRect);
	float getYRepairDistance(CCRect);
	CCRect getCurrentRect();
	void doTouch();
	void jump();
	void jumpAgain();

	CREATE_FUNC(Role);
	
	//attribute
	CC_PROPERTY(ROLE_STATE,currentRoleState,CurrentRoleState);
	CC_SYNTHESIZE(CCSprite *,mRole,MRole);
	CC_SYNTHESIZE(float,xSpeed,XSpeed);
	CC_SYNTHESIZE(float,ySpeed,YSpeed);
	CC_SYNTHESIZE(float,currentXSpeed,CurrentXSpeed);
	CC_SYNTHESIZE(float,currentYSpeed,CurrentYSpeed);
private:
	float gravity;
	float maxYSpeed;
	ActionManager * actionManager;
	void roleXGo();
	void roleXGo(float);
	void roleYGo();
	void roleYGo(float);
};

