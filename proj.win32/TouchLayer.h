#pragma once
#include "MHeader.h"
#include "Role.h"

class TouchLayer :
	public CCLayer
{
public:
	TouchLayer(void);
	~TouchLayer(void);
	CREATE_FUNC(TouchLayer);
	virtual void onEnter();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	CC_PROPERTY(Role *,myHandle,MyHandle);
};

