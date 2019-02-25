#include "Ufo.h"

Ufo* Ufo ::create(UfoType t)
{
	auto pRet = new Ufo();
	if(pRet && pRet->init(t))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Ufo::init(UfoType te)
{
	bool isRight = false;
	m_type = te;
	switch (m_type)
	{
	case SUPERBULLET:
		isRight = Sprite::initWithSpriteFrameName("shuangzidan.png");
		break;
	case SUPERBOMB:
		isRight = Sprite::initWithSpriteFrameName("zhadandj.png");
		break;
	case SUPERLIGHT:
		isRight = Sprite::initWithSpriteFrameName("wudi.png");
		break;
	case BLOOD:
		isRight = Sprite::initWithSpriteFrameName("yaoping.png");
		break;
	default:
		break;
	}
	return isRight;
}
