#pragma once
#include "cocos2d.h"
#include "commData.h"
using namespace cocos2d;

class Ufo :public Sprite
{
public:
	static Ufo* create(UfoType);
	bool init(UfoType);
	UfoType m_type;
};
