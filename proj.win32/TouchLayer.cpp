#include "TouchLayer.h"


TouchLayer::TouchLayer(void)
{
	myHandle = NULL;
}


TouchLayer::~TouchLayer(void)
{
	CC_SAFE_RELEASE(myHandle);
}

void TouchLayer::onEnter()
{
	CCLayer::onEnter();
	CCLOG("Enter");
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
}

bool TouchLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("Begin");
	return true;
}

void TouchLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("Ended");
	myHandle->doTouch();
}

Role * TouchLayer::getMyHandle()
{
	return myHandle;
}

void TouchLayer::setMyHandle(Role * _handle)
{
	if(myHandle)
		CC_SAFE_RELEASE(myHandle);
	myHandle = _handle;
	CC_SAFE_RETAIN(myHandle);
}