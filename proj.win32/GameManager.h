#pragma once
#include "MHeader.h"
#include "Role.h"
#include "TouchLayer.h"
#include "Vector"
class GameManager :
	public CCNode
{
public:
	GameManager(void);
	~GameManager(void);
	
	CREATE_FUNC(GameManager);
	virtual bool init();
	void initRole();
	bool isCrash();
	bool checkCrash(CCRect);
	virtual void update(float);
	void cameraRun();
	void gameRun();
	void gameover();
	void initCoins();
	void checkCoins();

	CCSpriteBatchNode * coinBatchNode;

	std::vector<CCSprite *> coinVector;
	CC_SYNTHESIZE(Role *,_role,Role);
	CC_SYNTHESIZE(CCTMXTiledMap*, _tileMap,TileMap);
	CC_SYNTHESIZE(TouchLayer *,touchLayer,TouchLayer);
};

