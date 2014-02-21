#include "GameManager.h"

GameManager::GameManager(void)
{
	coinBatchNode = CCSpriteBatchNode::create("coin.png");
	CC_SAFE_RETAIN(coinBatchNode);
}

GameManager::~GameManager(void)
{
	CC_SAFE_RELEASE(coinBatchNode);
	for ( int i = 0 ; i < coinVector.size() ; i++ )
	{
		CC_SAFE_RELEASE(coinVector[i]);
	}
	coinVector.clear();
}

bool GameManager::init()
{
	bool ret = false;
	do
	{
		CC_BREAK_IF( !CCNode::init() );
		currentScore = 0;

		_tileMap = CCTMXTiledMap::create("map.tmx");
		this->addChild( _tileMap , 0);

		gameScoreLabel = CCLabelTTF::create("0",  "Arial", 30);
		gameScoreLabel->setColor(ccORANGE);
		gameScoreLabel->setVisible(true);
		this->addChild(gameScoreLabel, 1);

		gameoverLabel = CCLabelTTF::create("0",  "Arial",60);
		gameoverLabel->setColor(ccRED);
		gameoverLabel->setString("Game Over!");
		gameoverLabel->setVisible(false);
		this->addChild(gameoverLabel, 1);

		initRole();

		initCoins();

		touchLayer = TouchLayer::create();
		touchLayer->setMyHandle(_role);
		this->addChild(touchLayer , 3);
		cameraRun();
		scheduleUpdate();
		ret = true;
	}while(0);
	return ret;
}

void GameManager::initCoins()
{
	CCTMXObjectGroup* roleGroups = _tileMap->objectGroupNamed("coins");
	CCObject * obj = NULL;
	CCARRAY_FOREACH(roleGroups->getObjects(),obj)
	{
		CCDictionary* dic = (CCDictionary*)obj;
		float x = dic->valueForKey("x")->floatValue();
		float y = dic->valueForKey("y")->floatValue();
		
		Coin * coin = Coin::createWithTexture(coinBatchNode->getTexture());
		coin->setAnchorPoint(CCPointZero);
		coin->setPosition(ccp(x,y));
		coin->setScore( ((CCString*)dic->objectForKey("score"))->intValue() );
		CC_SAFE_RETAIN(coin);
		coinVector.push_back(coin);
		_tileMap->addChild(coin);
	}
}

void GameManager::initRole()
{
	do
	{
		_role = Role::create();
		CC_BREAK_IF( !_role );
		CCTMXObjectGroup* roleGroups = _tileMap->objectGroupNamed("role");
		CCObject * obj = NULL;
		CCARRAY_FOREACH(roleGroups->getObjects(),obj)
		{
			CCDictionary* dic = (CCDictionary*)obj;
			if(dic->valueForKey("name")->compare("start") == 0){
				float x = dic->valueForKey("x")->floatValue();
				float y = dic->valueForKey("y")->floatValue();
				_role->setRolePosition(ccp(x,y));
			}
		}
		this->addChild( _role , 1);
		
	}while(0);
}

//检测接下来是否会碰撞,如果将碰撞,直接将位置调整至合适处,同时需要改变状态
bool GameManager::isCrash()
{
	CCRect rect = _role->getNextRect();//获取下一帧将在的位置
	return checkCrash(rect);
}

bool GameManager::checkCrash(CCRect nextRect)
{
	CCTMXObjectGroup* roleGroups = _tileMap->objectGroupNamed("walls");
	CCObject * obj = NULL;
	bool crashed = false;//表示是否发生碰撞

	float minDeltaX = CCDirector::sharedDirector()->getWinSize().width;
	float minDeltaY = CCDirector::sharedDirector()->getWinSize().height;

	CCARRAY_FOREACH(roleGroups->getObjects(),obj)
	{
		CCDictionary* dic = (CCDictionary*)obj;
		if(dic->valueForKey("name")->compare("wall") == 0)
		{
			float x = dic->valueForKey("x")->floatValue();
			float y = dic->valueForKey("y")->floatValue();
			float width = dic->valueForKey("width")->floatValue();
			float height = dic->valueForKey("height")->floatValue();
			CCRect targetRect = CCRectMake(x,y,width,height);
			
			if (nextRect.intersectsRect(targetRect))//将会发生碰撞
			{
				float deltaX = _role->getXRepairDistance(targetRect); //获取与碰撞物体间的x距离
				if(deltaX > -1) 
					minDeltaX = MIN(minDeltaX , deltaX);//取x距离的最小值

				float deltaY = _role->getYRepairDistance(targetRect);//获取与碰撞物体间的y距离
				if(deltaY >= -1) 
					minDeltaY = MIN(minDeltaY , deltaY);//取y距离的最小值

				crashed = true;
			}
		}
	}
	if(crashed)//如果将发生碰撞将需要修复位置
		_role->roleGo(minDeltaX,minDeltaY);

	return crashed;
}

void GameManager::update(float delta)
{
	if(_role->getCurrentRoleState() == ROLE_DEAD)
		gameover();
	gameRun();
	cameraRun();
}

void GameManager::gameRun()
{
	checkCoins();
	if(!isCrash())
		_role->roleGo();
	scoreDisplay();
}

void GameManager::checkCoins()
{
	for ( int i = 0 ; i < coinVector.size() ; i++ )
	{
		if(coinVector[i]->boundingBox().intersectsRect(_role->getCurrentRect()))
		{
			currentScore += coinVector[i]->getScore();
			coinVector[i]->removeFromParentAndCleanup(true);
			coinVector.erase(coinVector.begin() + i);
			i--;
		}
	}
}

//设置摄像头的位置，摄像头应该保持横向匀速运动;纵向跟随，使角色位于中心位置；
void GameManager::cameraRun()
{
	float x = this->getPositionX() - _role->getXSpeed();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	float mapHeight = _tileMap->getTileSize().height * _tileMap->getMapSize().height;
	float y = MAX(_role->getPositionY(),s.height / 2);
	y = MIN(y , mapHeight - s.height / 2);
	this->setPosition(x , y - s.height / 2);
	if(_role->getCurrentRect().getMaxX() < -getPositionX() || _role->getCurrentRect().getMaxY() < -getPositionY())//当角色不在摄像头范围内时，角色死亡
	{
		_role->setCurrentRoleState(ROLE_DEAD);
		return;
	}

	gameScoreLabel->setPosition(ccp( 20 - getPositionX(),s.height - getPositionY() - 20 ));
	gameoverLabel->setPosition(ccp( s.width / 2 - getPositionX() ,s.height / 2 - getPositionY()));

	//当位置离1/4 屏幕较远时，需要考虑加速
	float dis = (s.width / 4 - getPositionX() - _role->getCurrentRect().getMaxX()) ;//获取距离屏幕 1/4的长度
	float dx = dis / 90;//使速度平滑的减到 xspeed
	if(dx <= 0.0001){
		_role->setCurrentXSpeed(_role->getXSpeed() + dis);
	} else {
		_role->setCurrentXSpeed(_role->getXSpeed() + dx);
	}
}

void GameManager::gameover()
{
	gameoverLabel->setVisible(true);
	this->pauseSchedulerAndActions();
}

void GameManager::scoreDisplay()
{
	char str[100] = {'0'};  
	sprintf(str, "%d", currentScore);
	gameScoreLabel->setString(str);
}