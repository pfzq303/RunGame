#pragma once
#include "MHeader.h"
class Coin :
	public CCSprite
{
public:
	Coin(void);
	~Coin(void);
	static Coin* createWithTexture(CCTexture2D * texture)
	{
		Coin * coin = new Coin();
		if (coin && coin->initWithTexture(texture))
		{
			coin->autorelease();
			return coin;
		}
		CC_SAFE_DELETE(coin);
		return NULL;
	}
	CC_SYNTHESIZE(int , score,Score);
};

