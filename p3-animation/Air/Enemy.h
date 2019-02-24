#pragma once

#include "commData.h"

class Enemy :public Sprite
{
public:
	static Enemy* create(EnemyType);
	bool init(EnemyType);
	EnemyType m_type;
	int getHp();
	void setHp(int);
	void EnemyHit();
	void EnemyBlow();
	float m_EnemyStep;
	float getEnemyStep();
	int m_Hp;
	int getScore(EnemyType);

	static float m_levelSpeed;
	static void incrementSpeed();
	static void clearLevelSpeed();

};

