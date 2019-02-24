#include "Bullet.h"


Bullet*  Bullet::create(BulletType t)
{
	auto pRet = new Bullet();
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

bool Bullet::init(BulletType te)
{
	bool isRight = false;
	m_type = te;
	switch (m_type)
	{
	case SINGLEBULLET:
		isRight = Sprite::initWithSpriteFrameName("zidan1.png");
		break;
	case DOUBLEBULLET:
		isRight = Sprite::initWithSpriteFrameName("zidan2.png");
		break;
	case LASER:
		isRight = Sprite::initWithSpriteFrameName("jiguang2.png");
		break;
	case BOSS_BULLET:
		isRight = Sprite::initWithSpriteFrameName("zidan3.png");
		break;
	default:
		break;
	}
	return isRight;
}


void Bullet::BulletBlow()
{
	auto animation = Animation::create();

	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("jiguang2.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("jiguang1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("jiguang2.png"));

	this->stopAllActions();
	animation->setDelayPerUnit(0.2f);
	auto animate = Animate::create(animation);
	auto callFunc = CallFuncN::create([=](Node* node)
		{
			this->removeFromParentAndCleanup(true);
	});
	this->runAction(Sequence::create(animate,callFunc,NULL));
}
