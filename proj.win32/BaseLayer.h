#pragma once
#include "MHeader.h"
class BaseLayer :
	public CCLayer
{
public:
	BaseLayer(void);
	~BaseLayer(void);
	virtual bool init();
	CREATE_FUNC(BaseLayer);
	static CCScene * scene();
};

