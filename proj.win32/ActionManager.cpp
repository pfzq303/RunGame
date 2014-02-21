#include "ActionManager.h"


ActionManager::ActionManager(void)
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("roleRun.png", CCRectMake(0,0, 32,32)), "up_1");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("roleRun.png", CCRectMake(32,0, 32,32)), "up_2");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("roleRun.png", CCRectMake(64,0, 32,32)), "up_3");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("roleRun.png", CCRectMake(96,0, 32,32)), "up_4");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("role_down.png", CCRectMake(0,0, 32,32)), "down_1");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("role_down.png", CCRectMake(32,0, 32,32)), "down_2");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("role_down.png", CCRectMake(64,0, 32,32)), "down_3");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(CCSpriteFrame::create("role_down.png", CCRectMake(96,0, 32,32)), "down_4");
}


ActionManager::~ActionManager(void)
{
}

CCAction* ActionManager::getRoleAction(ROLE_STATE currentState,ROLE_STATE nextState)
{
	CCAction * action = NULL;
	CCArray* upArray = CCArray::create(); 
	upArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("up_1"));
	upArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("up_2"));
	upArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("up_3"));
	upArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("up_4"));
	CCArray* downArray = CCArray::create(); 
	downArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("down_1"));
	downArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("down_2"));
	downArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("down_3"));
	downArray->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("down_4"));
	CCAnimation *animation1 = NULL,*animation2 = NULL;
	CCAnimate *animate = NULL;
	switch(nextState)
	{
	case ROLE_SECOND_UP:case ROLE_FIRST_UP:
		action = CCAnimate::create(CCAnimation::createWithSpriteFrames(upArray,  1/60.0));
		break;
	case ROLE_SECOND_DOWN:case ROLE_FIRST_DOWN:
		upArray->reverseObjects();
		action = CCSequence::create(	CCAnimate::create(CCAnimation::createWithSpriteFrames(upArray, 1/60.0)),
										CCAnimate::create(CCAnimation::createWithSpriteFrames(downArray,  1/60.0)),NULL);
		break;
	case ROLE_RUN:
		if(currentState == ROLE_FIRST_DOWN || currentState == ROLE_SECOND_DOWN)
		{
			downArray->reverseObjects();
			action = CCAnimate::create(CCAnimation::createWithSpriteFrames(downArray, 2/60.0));
		}
		break;
	}
	return action;
}