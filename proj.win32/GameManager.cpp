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
		_tileMap = CCTMXTiledMap::create("map.tmx");
		this->addChild( _tileMap , 0);
		initRole();
		initCoins();
		touchLayer = TouchLayer::create();
		touchLayer->setMyHandle(_role);
		this->addChild(touchLayer , 3);
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
		CCSprite * sprite = CCSprite::createWithTexture(coinBatchNode->getTexture());
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(x,y));
		CC_SAFE_RETAIN(sprite);
		coinVector.push_back(sprite);
		_tileMap->addChild(sprite);
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

//���������Ƿ����ײ,�������ײ,ֱ�ӽ�λ�õ��������ʴ�,ͬʱ��Ҫ�ı�״̬
bool GameManager::isCrash()
{
	CCRect rect = _role->getNextRect();//��ȡ��һ֡���ڵ�λ��
	return checkCrash(rect);
}

bool GameManager::checkCrash(CCRect nextRect)
{
	CCTMXObjectGroup* roleGroups = _tileMap->objectGroupNamed("walls");
	CCObject * obj = NULL;
	bool crashed = false;//��ʾ�Ƿ�����ײ

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
			
			if (nextRect.intersectsRect(targetRect))//���ᷢ����ײ
			{
				float deltaX = _role->getXRepairDistance(targetRect); //��ȡ����ײ������x����
				if(deltaX > -1) 
					minDeltaX = MIN(minDeltaX , deltaX);//ȡx�������Сֵ

				float deltaY = _role->getYRepairDistance(targetRect);//��ȡ����ײ������y����
				if(deltaY >= -1) 
					minDeltaY = MIN(minDeltaY , deltaY);//ȡy�������Сֵ

				crashed = true;
			}
		}
	}
	if(crashed)//�����������ײ����Ҫ�޸�λ��
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
}

void GameManager::checkCoins()
{
	for ( int i = 0 ; i < coinVector.size() ; i++ )
	{
		if(coinVector[i]->boundingBox().intersectsRect(_role->getCurrentRect()))
		{
			coinVector[i]->removeFromParentAndCleanup(true);
			coinVector.erase(coinVector.begin() + i);
			i--;
		}
	}
}

//��������ͷ��λ�ã�����ͷӦ�ñ��ֺ��������˶�;������棬ʹ��ɫλ������λ�ã�
void GameManager::cameraRun()
{
	float x = this->getPositionX() - _role->getXSpeed();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	float mapHeight = _tileMap->getTileSize().height * _tileMap->getMapSize().height;
	float y = MAX(_role->getPositionY(),s.height / 2);
	y = MIN(y , mapHeight - s.height / 2);
	this->setPosition(x , y - s.height / 2);
	if(_role->getCurrentRect().getMaxX() < -getPositionX() || _role->getCurrentRect().getMaxY() < -getPositionY())//����ɫ��������ͷ��Χ��ʱ����ɫ����
	{
		_role->setCurrentRoleState(ROLE_DEAD);
		return;
	}
	// coding
	//��λ����1/4 ��Ļ��Զʱ����Ҫ���Ǽ���
	float dis = (s.width / 4 - getPositionX() - _role->getCurrentRect().getMaxX()) ;//��ȡ������Ļ 1/4�ĳ���
	float dx = dis / 90;//ʹ�ٶ�ƽ���ļ��� xspeed
	if(dx <= 0.0001){
		_role->setCurrentXSpeed(_role->getXSpeed() + dis);
	} else {
		_role->setCurrentXSpeed(_role->getXSpeed() + dx);
	}
}

void GameManager::gameover()
{
	this->pauseSchedulerAndActions();
}