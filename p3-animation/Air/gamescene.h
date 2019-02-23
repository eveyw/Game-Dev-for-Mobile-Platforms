#pragma once
#include "cocos2d.h"


USING_NS_CC;

class gamescene :public cocos2d::Layer
{
public:
	gamescene();
	static cocos2d::Scene* createScene();
	static gamescene* create();
    bool init();
	void update(float);

	
};
