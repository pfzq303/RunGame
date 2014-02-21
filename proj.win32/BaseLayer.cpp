#include "BaseLayer.h"
#include "GameManager.h"

BaseLayer::BaseLayer(void)
{
}


BaseLayer::~BaseLayer(void)
{
}

	
bool BaseLayer::init()
{
	bool ret = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		GameManager* gameManager = GameManager::create();
		CC_BREAK_IF(!gameManager);
		this->addChild(gameManager);
		ret = true;
	}while(0);
	return ret;
}
CCScene * BaseLayer::scene()
{
	CCScene * scene = CCScene::create();
	do
	{
		CC_BREAK_IF(!scene);
		BaseLayer* layer = BaseLayer::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	}while(0);
	return scene;
}