#include "Role.h"
#include "ActionManager.h"

Role::Role(void):currentRoleState(ROLE_FIRST_DOWN),xSpeed(1.5),ySpeed(10),
	gravity(0.5),maxYSpeed(15)
{
	actionManager = ActionManager::create();
	CC_SAFE_RETAIN(actionManager);
}


Role::~Role(void)
{
	CC_SAFE_RELEASE(actionManager);
}

bool Role::init()
{
	bool ret = false;
	do
	{
		CC_BREAK_IF(!CCNode::init());
		currentXSpeed = xSpeed;
		currentYSpeed = 0;

		mRole = CCSprite::create("role.png");
		CC_BREAK_IF(!mRole);
		mRole->setAnchorPoint(CCPointZero);
		this->addChild(mRole);

		ret = true;
	}while(0);
	return ret;
}

void Role::setRolePosition(CCPoint _point)
{
	mRole->setPosition(_point);
}

void Role::roleGo()
{
	roleXGo();
	roleYGo();
}

void Role::roleGo(float dx,float dy)
{
	roleXGo(dx);
	roleYGo(dy);
}

void Role::roleXGo()
{
	float x = mRole->getPositionX() + ROLE_DIR[(int)currentRoleState].x * currentXSpeed;
	mRole->setPositionX(x);
}

void Role::roleXGo(float dx)
{
	float movX = ROLE_DIR[(int)currentRoleState].x * currentXSpeed;
	if(movX > dx){
		mRole->setPositionX(mRole->getPositionX() + dx);
	} else {
		roleXGo();
	}
}

void Role::roleYGo()//在未发生碰撞时的运行方式
{

	currentYSpeed = currentYSpeed + ROLE_DIR[(int)currentRoleState].y * -1 * gravity;//速度受重力影响
	currentYSpeed = MIN (currentYSpeed ,maxYSpeed);

	float y = mRole->getPositionY() + ROLE_DIR[(int)currentRoleState].y * currentYSpeed;
	mRole->setPositionY(y);

	if(currentYSpeed <= 0)//速度为0时需要改变状态为下落
	{
		currentYSpeed = 0;
		if(currentRoleState == ROLE_FIRST_UP)
			setCurrentRoleState(ROLE_FIRST_DOWN);
		else if(currentRoleState == ROLE_SECOND_UP)
			setCurrentRoleState(ROLE_SECOND_DOWN);
	}
}

void Role::roleYGo(float dy)
{
	if(ROLE_DIR[(int)currentRoleState].y > 0)
	{
		float movY = ROLE_DIR[(int)currentRoleState].y * currentYSpeed;
		if(movY >= dy) //向上发生碰撞
		{
			mRole->setPositionY(mRole->getPositionY() + dy);
			if(currentRoleState != ROLE_FIRST_DOWN)
				setCurrentRoleState(ROLE_FIRST_DOWN);//反弹，速度大小不变，反向
		} else {
			roleYGo();
		}
	} else if(ROLE_DIR[(int)currentRoleState].y < 0)
	{
		float movY = -ROLE_DIR[(int)currentRoleState].y * currentYSpeed;
		if(movY >= dy) //向下发生碰撞
		{
			if(dy > 0)
				mRole->setPositionY(mRole->getPositionY() - dy);
			if(currentRoleState != ROLE_RUN)
				setCurrentRoleState(ROLE_RUN);
		} else {
			roleYGo();
		}
	}
	
}

CCRect Role::getNextRect()
{
	CCRect rect = getCurrentRect();
	return CCRectMake(	rect.getMinX() + ROLE_DIR[(int)currentRoleState].x * currentXSpeed,
					rect.getMinY() + ROLE_DIR[(int)currentRoleState].y * currentYSpeed,
					rect.getMaxX() - rect.getMinX(),
					rect.getMaxY() - rect.getMinY());
}

CCRect Role::getCurrentRect()
{
	return mRole->boundingBox();
}

ROLE_STATE Role::getCurrentRoleState()
{
	return currentRoleState;
}

void Role::setCurrentRoleState(ROLE_STATE role_state){
	
	CCAction * action = actionManager->getRoleAction(currentRoleState,role_state);
	if(action){
		mRole->stopAllActions();
		mRole->runAction(action);
	}
	currentRoleState = role_state;
	switch(currentRoleState)
	{
	case ROLE_RUN:
		currentYSpeed = 0;
		break;
	case ROLE_FIRST_UP:case ROLE_SECOND_UP:
		currentYSpeed = ySpeed;
		break;
	};
	
}

float Role::getXRepairDistance(CCRect targetRect)
{
	CCRect rect = getCurrentRect();
	float x = 0;
	if(ROLE_DIR[(int)currentRoleState].x > 0){ //如果朝右
		x = targetRect.getMinX() - rect.getMaxX();//获取两个rect的x方向差值
	}
	return x;
}

float Role::getYRepairDistance(CCRect targetRect)
{
	CCRect rect = getCurrentRect();
	float y = mRole->getPositionY();
	float deltaY = 0;
	if(ROLE_DIR[(int)currentRoleState].y > 0){//朝上
		deltaY = targetRect.getMinY() - rect.getMaxY() ;
	} 
	if(ROLE_DIR[(int)currentRoleState].y < 0){//朝下
		deltaY = rect.getMinY() - targetRect.getMaxY();
	}
	return deltaY;
}

void Role::doTouch()
{
	switch(currentRoleState)
	{
		case ROLE_FIRST_UP:case ROLE_FIRST_DOWN://在第一次跳跃过程中可以跳第二次
			jumpAgain();
			break;
		case ROLE_RUN:
			jump();
			break;
		default:
			break;
	};
}

void Role::jump()
{
	setCurrentRoleState(ROLE_FIRST_UP);
}

void Role::jumpAgain()
{
	setCurrentRoleState(ROLE_SECOND_UP);
}