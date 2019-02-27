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
	
    void createSmall(float);
    void createBig(float);
    void createMid(float);
    
    void createUfos(UfoType);
    Vector<Ufo*> m_vUfos;
    void createSuperBullet(float);
    void createSuperBomb(float);
    
    bool m_isDoubleBullet;
    int m_doubleBulletNumber;
    
    bool m_isSuperBomb;
    int m_SuperBombNumber;
    bool m_isbomb;
    void updateBombLabel();
    
    int SumScore;
    
    void pause(Ref *);
    bool m_canMove;
    void resume(Ref *);
    
    bool m_planeActive;
    
    void updateEnergyLabel();
    float m_EnergyNumber;
    bool m_isLaser;
    
    void L_LevelUp();
    int LevelCount;
    bool m_isMenuShow;
    void RePlay(Ref *);
    void Exit(Ref *);
    
    int delaytime;
    bool isjia;
    int hero_blood;
    void herohit();
    
    bool is_undead;
    void heroBlowup();
    Vector<Bullet*> enem2_vBulets;
    cocos2d::Vector<Bullet*> enem3_vBulets;
    cocos2d::Vector<Bullet*> enem4_vBulets;
    cocos2d::Vector<Bullet*> enem5_vBulets;
    bool order;
    void bloodupdate();
    void createUfo3(float);
    void createBlood(float);
    void createSuperLight(float);
    void createEnemy_bul(float);
    Vector<Bullet*> enem_vBulets;
};
