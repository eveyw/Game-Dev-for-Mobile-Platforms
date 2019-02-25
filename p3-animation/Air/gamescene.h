#pragma once
#include "cocos2d.h"
#include "Bullet.h"
#include "Enemy.h"

USING_NS_CC;

class gamescene :public cocos2d::Layer
{
public:
	gamescene();
	static cocos2d::Scene* createScene();
	static gamescene* create();
    bool init();
	void update(float);

    cocos2d::Point vec;
    void createBullet(float);
    void createLaser(float);
    Vector<Bullet*> m_vBullets;
    Vector<Bullet*> m_vLaser;
    
    void createEnemy(EnemyType);
    Vector<Enemy*> m_vEnemys;
	
};
