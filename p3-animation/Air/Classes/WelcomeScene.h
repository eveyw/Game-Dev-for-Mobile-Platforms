#pragma once
#include "gamescene.h"
#include "commData.h"


class WelcomeScene : public Layer
{
public:
	bool init();
	static WelcomeScene* create();

	static Scene *createScene();
};
