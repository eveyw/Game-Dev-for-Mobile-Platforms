#include "Enemy.h"

#include "commData.h"

float Enemy::m_levelSpeed = 0;

Enemy* Enemy ::create(EnemyType t)
{
	auto pRet = new Enemy();
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

bool Enemy::init(EnemyType te)
{
	bool isRight = false;
	m_type = te;
	switch (m_type)
	{
	case SMALLENEMY:
		isRight = Sprite::initWithSpriteFrameName("enemy1.png");
		this->m_EnemyStep = smallStep;
		this->m_Hp = 1;
		break;
	case MIDENEMY:
		isRight = Sprite::initWithSpriteFrameName("enemy2.png");
		this->m_EnemyStep = midStep;
		this->m_Hp = 4;
		break;
	case BIGENEMY:
		{
			isRight = Sprite::initWithSpriteFrameName("enemy3.png");
			this->m_EnemyStep = bigStep;
			this->m_Hp = 8;
			break;
		}
	default:
		break;
	}
	return isRight;
}

int Enemy::getHp()
{
	return m_Hp;
}
void Enemy::setHp(int hp)
{
	m_Hp = hp;
}

float Enemy::getEnemyStep()
{
	return this->m_EnemyStep + Enemy::m_levelSpeed;;
}

void Enemy:: EnemyHit()
{
	this->m_Hp --;
	if (this->m_Hp <= 0)
	{
		this->EnemyBlow();
		return;
	}
	auto animation = Animation::create();

	if(this->m_type==MIDENEMY)
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2hit.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
	}
	else
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3hit.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3.png"));
	}
	animation->setDelayPerUnit(0.2);
	//auto animate = Animate::create(animation);
	this->runAction(Animate::create(animation));
			
}

void Enemy::EnemyBlow()
{
	auto animation = Animation::create();

	if(this->m_type==SMALLENEMY)
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1hit.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1blow.png"));
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
		SimpleAudioEngine::getInstance()->playEffect("enemy1_down.mp3");

	}
	else if(this->m_type==MIDENEMY)
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2hit.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2blow.png"));
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
		SimpleAudioEngine::getInstance()->playEffect("enemy2_down.mp3");

	}
	else
	{
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3blow.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3hit.png"));
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
		SimpleAudioEngine::getInstance()->playEffect("enemy3_down.mp3");

	}
	this->stopAllActions();
	animation->setDelayPerUnit(0.2f);
	auto animate = Animate::create(animation);
	auto callFunc = CallFuncN::create([=](Node* node)
		{
			this->removeFromParentAndCleanup(true);
	});
	this->runAction(Sequence::create(animate,callFunc,NULL));
}

int Enemy::getScore(EnemyType te)
{
	m_type = te;
	switch (m_type)
	{
	case SMALLENEMY:
		return 1;
	case MIDENEMY:
		return 4;
	case BIGENEMY:
		return 8;
	default:
		break;
	}
	return 0;
}

void Enemy::incrementSpeed() {
	if (Enemy::m_levelSpeed ==6)
	{
		return;
	}
	Enemy::m_levelSpeed += 0.5f;
}
void Enemy::clearLevelSpeed()
{
	Enemy::m_levelSpeed = 0;
}
