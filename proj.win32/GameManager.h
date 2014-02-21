#pragma once
#include "Role.h"
#include "TouchLayer.h"
#include "Vector"
#include "Coin.h"

class GameManager :
	public CCNode
{
public:
	GameManager(void);
	~GameManager(void);
	
	CREATE_FUNC(GameManager);
	virtual bool init();
	virtual void update(float);
	
	CC_SYNTHESIZE(Role *,_role,Role);
	CC_SYNTHESIZE(CCTMXTiledMap*, _tileMap,TileMap);
	CC_SYNTHESIZE(TouchLayer *,touchLayer,TouchLayer);

private:
	CCLabelTTF * gameScoreLabel;
	CCLabelTTF * gameoverLabel;
	int currentScore;
	CCSpriteBatchNode * coinBatchNode;
	std::vector<Coin *> coinVector;

	void initRole();
	bool isCrash();
	bool checkCrash(CCRect);
	void cameraRun();
	void gameRun();
	void gameover();
	void initCoins();
	void checkCoins();
	void scoreDisplay();
};

