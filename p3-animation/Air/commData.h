#pragma once
#include "cocos2d.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define visibleSize Director::getInstance()->getVisibleSize()
#define smallStep  4
#define midStep  2
#define bigStep 1

enum EnemyType
{
	SMALLENEMY,
	MIDENEMY,
	BIGENEMY,	
};

enum BulletType
{
	SINGLEBULLET,
	DOUBLEBULLET,
	LASER,
	BOSS_BULLET,
};
