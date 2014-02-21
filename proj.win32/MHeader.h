#ifndef __MHEAD_H__
#define __MHEAD_H__

#include "cocos2d.h"

USING_NS_CC;

enum ROLE_STATE
{
	ROLE_RUN,
	ROLE_FIRST_UP,
	ROLE_SECOND_UP,
	ROLE_FIRST_DOWN,
	ROLE_SECOND_DOWN,
	ROLE_DEAD
};
const CCPoint ROLE_DIR[] = {
	ccp(1,-1),
	ccp(1,1),
	ccp(1,1),
	ccp(1,-1),
	ccp(1,-1),
	ccp(0,0)
}; 

#endif