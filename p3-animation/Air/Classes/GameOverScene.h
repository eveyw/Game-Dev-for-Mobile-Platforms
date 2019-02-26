#pragma once
#include "cocos2d.h"
#include "commData.h"
#include "gamescene.h"

using namespace cocos2d;

class GameOverScene : public Layer
{
public:
	bool init(int);
	static GameOverScene* create(int);

	static Scene *createScene(int);
	int highScore;
	GameOverScene();
};

