#pragma once
#include "commData.h"

class Bullet :public cocos2d::Sprite
{
public:
	static Bullet* create(BulletType);
	bool init(BulletType t);
	BulletType m_type;
	void BulletBlow();
};


