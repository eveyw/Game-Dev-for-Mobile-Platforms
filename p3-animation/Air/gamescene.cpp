#include "gamescene.h"


//int gamescene::SumsScore = 0
gamescene::gamescene()
{
	vec = Point::ZERO;
	m_isDoubleBullet = false;
	m_isSuperBomb = false;
	m_SuperBombNumber = 0 ;
	SumScore = 0;
	m_canMove = true;
	m_planeActive = true;
	m_EnergyNumber = 0;
	m_isLaser = false;
	LevelCount = 1;
	m_isMenuShow = false;
	m_isbomb = false;
	is_undead = false;
	isjia = false;
	hero_blood = 6;
	delaytime = 0;
}

Scene* gamescene::createScene()
{
	auto scene = Scene::create();
	auto lay = gamescene::create();
	scene->addChild(lay);
	return scene;
}

gamescene* gamescene::create()
{
	auto gma = new gamescene;
	if(gma && gma->init())
	{
		gma->autorelease();
		return gma;
	}
	else
	{
		delete gma;
		gma = nullptr;
		return nullptr;
	}
}
bool gamescene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::createWithSpriteFrameName("background.png");
	//bg->setPosition(200,200);
	bg->setAnchorPoint(Point::ZERO);

	this->addChild(bg,0,1);


	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Point::ZERO);
	bg2->setPositionY(bg->getContentSize().height);
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2,0,2);
	

	auto lblScore = Label::createWithSystemFont("Score:", "Arial", 36);
	lblScore->setPosition(40, visibleSize.height-40);
	lblScore->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblScore->setColor(Color3B::BLUE);
	this->addChild(lblScore, 99);

	auto lblScoreNumber = Label::createWithBMFont("font.fnt", "0");
	lblScoreNumber->setPosition(lblScore->getContentSize().width + 45, visibleSize.height-40);
	lblScoreNumber->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblScoreNumber->setColor(Color3B::BLUE);
	this->addChild(lblScoreNumber, 99, 90);


	auto hero = Sprite::createWithSpriteFrameName("human.png");
	hero->setAnchorPoint(Point(0.5,0.5));
	hero->setPosition(visibleSize.width/2,hero->getContentSize().height/2);
	this->addChild(hero,1,3);

	auto dun = Sprite::createWithSpriteFrameName("wudi1.png");
	dun->setVisible(false);
	this->addChild(dun,1,66);


	auto lis = EventListenerTouchOneByOne::create();
	lis->onTouchBegan = [=](Touch* t,Event*)
	{
		auto pos = t->getLocation();
		bool isContain = hero->getBoundingBox().containsPoint(pos);
		vec = hero->getPosition()- pos;

		return isContain && m_canMove;
	};
	lis->onTouchMoved = [=](Touch* t,Event* e)
	{
		auto pos = t->getLocation();
		auto nwPos = pos+vec;
		auto minX = hero->getContentSize().width/2;
		auto maxX = visibleSize.width - minX;
		auto minY = hero->getContentSize().height/2;
		auto maxY = visibleSize.height - minY;

		auto x = MAX(MIN(nwPos.x,maxX),minX);
		auto y = MAX(MIN(nwPos.y,maxY),minY);
		hero->setPosition(x,y);
		if(isjia)
		{
			dun->setPosition(x,y);
			dun->setVisible(true);
		}
		else
		{
			dun->setVisible(false);
		}
		log("This is touch moved");
		
	};
	lis->onTouchEnded = [](Touch*,Event*)
	{
		log("This is touch ended");
		
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis,hero);
	schedule(schedule_selector(gamescene::createBullet),0.2);
	schedule(schedule_selector(gamescene::createLaser),0.1);
	schedule(schedule_selector(gamescene::createSmall),0.8,-1,3);
	schedule(schedule_selector(gamescene::createMid),2,-1,6);
	schedule(schedule_selector(gamescene::createBig),10,-1,12);
	schedule(schedule_selector(gamescene::createSuperBullet),5,-1,7);
	schedule(schedule_selector(gamescene::createSuperBomb),8,-1,5);
	schedule(schedule_selector(gamescene::createEnemy_bul), 0.9, -1, 9);
	schedule(schedule_selector(gamescene::createSuperLight), 20, -1, 13);
	schedule(schedule_selector(gamescene::createBlood), 15, -1, 11);

	Enemy::clearLevelSpeed();
	this->scheduleUpdate();

	auto spriteBomb = Sprite::createWithSpriteFrameName("zhadan.png");
	auto menuItemBomb = MenuItemSprite::create(spriteBomb, spriteBomb, [=](Ref *){
		this->m_SuperBombNumber--;

		SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
		SimpleAudioEngine::getInstance()->playEffect("use_bomb.mp3");

		auto hero1=this->getChildByTag(3);
		auto spriteBomb3 = this->getChildByTag(22);
		spriteBomb3->setPosition(hero1->getPosition());

		this->updateBombLabel();

		m_isbomb = true;
		auto callFunc = CallFuncN::create([](Node *node){
		node->removeFromParentAndCleanup(true);
		});
	});
	menuItemBomb->setPosition(40, 40);
	menuItemBomb->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	auto menu = Menu::create();
	menu->addChild(menuItemBomb,99,100);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 99,101);
	auto lblBombCount = Label::createWithBMFont("font.fnt", "X0");
	lblBombCount->setPosition(menuItemBomb->getContentSize().width+45, 40);
	lblBombCount->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblBombCount->setColor(Color3B::RED);
	this->addChild(lblBombCount, 99, 91);
	updateBombLabel();


	auto spriteEnergy = Sprite::createWithSpriteFrameName("nengliangtiao.png");
	spriteEnergy->setPosition(visibleSize.width-30,60);
	this->addChild(spriteEnergy,0);

	auto spriteEnergy0 = Sprite::createWithSpriteFrameName("fashe.png");
	auto spriteEnergy1 = Sprite::createWithSpriteFrameName("mfashe.png");
	auto menuItemEnergy = MenuItemSprite::create(spriteEnergy0, spriteEnergy1, [=](Ref *){
		this->m_EnergyNumber = 0;
		m_isLaser = true;
		this->updateEnergyLabel();
		});

	auto menu_E = Menu::create();
	menuItemEnergy->setPosition(visibleSize.width-30, 120);
	menuItemEnergy->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	menu_E->addChild(menuItemEnergy,99,120);
	menu_E->setPosition(Point::ZERO);
	this->addChild(menu_E, 99,121);
	for(int i = 1;i<=5;i++)
	{
		auto spriteEnergyCount = Sprite::createWithSpriteFrameName("energy.png");
		spriteEnergyCount->setPosition(visibleSize.width-30,spriteEnergyCount->getContentSize().height/1.5*i+25);
		this->addChild(spriteEnergyCount,0,121+i);
	}
	updateEnergyLabel();

	
	auto menu_P = Menu::create();
	auto pauseNor = Sprite::createWithSpriteFrameName("zanting.png");
	auto pausePressed = Sprite::createWithSpriteFrameName("kaishi.png");

	auto menuItemPause = MenuItemSprite::create(pauseNor, pausePressed, CC_CALLBACK_1(gamescene::pause, this));
	menuItemPause->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	menuItemPause->setPosition(Point(visibleSize)-Point(20, 20));
	menuItemPause->setTag(111);
	menu_P->addChild(menuItemPause);
	menu_P->setPosition(Point::ZERO);
	this->addChild(menu_P, 99,110);	


	auto menu_R = Menu::create();
	auto resumeNor = Sprite::createWithSpriteFrameName("kaishi1.png");
	auto resumePressed = Sprite::createWithSpriteFrameName("kaishi1.png");
	auto menuItemResume = MenuItemSprite::create(resumeNor, resumePressed, CC_CALLBACK_1(gamescene::resume, this));
	menuItemResume->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	menuItemResume->setPosition(1.4*visibleSize.width/5,visibleSize.height/2+30);
	menuItemResume->setTag(113);
	menuItemResume->setVisible(false);
	menu_R->addChild(menuItemResume);
	menu_R->setPosition(Point::ZERO);
	this->addChild(menu_R, 99,112);	
	
	auto menu_B=Menu::create();
	auto rePlayNor = Sprite::createWithSpriteFrameName("restart1.png");
	auto rePlayPressed = Sprite::createWithSpriteFrameName("restart1.png");
	auto menuItemRePlay = MenuItemSprite::create(rePlayNor, rePlayPressed,CC_CALLBACK_1(gamescene::RePlay, this));
	menuItemRePlay->setPosition(2.5*visibleSize.width/5,visibleSize.height /2);
	menuItemRePlay->setTag(115);
	menuItemRePlay->setVisible(false);
	menu_B ->addChild(menuItemRePlay);
	menu_B->setPosition(Point::ZERO);
	this->addChild(menu_B, 99,114);	

	auto menu_Ex = Menu::create();
	auto exitNor = Sprite::createWithSpriteFrameName("tuichu1.png");
	auto exitPressed = Sprite::createWithSpriteFrameName("tuichu1.png");
	auto menuItemExit = MenuItemSprite::create(exitNor, exitPressed, CC_CALLBACK_1(gamescene::Exit, this));
	menuItemExit->setTag(117);
	menuItemExit->setPosition(4*visibleSize.width/5,visibleSize.height /2);
	menuItemExit->setVisible(false);
	menu_Ex->addChild(menuItemExit);
	menu_Ex->setPosition(Point::ZERO);
	this->addChild(menu_Ex, 99,116);	

	auto spriteBomb2 = Sprite::createWithSpriteFrameName("zhadan.png");
	spriteBomb2->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	this->addChild(spriteBomb2,70,22);




	auto yaoping = Sprite::createWithSpriteFrameName("xue.png");
	yaoping->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);   //ÑªÁ¿Ìõ
	yaoping->setPosition(40+yaoping->getContentSize().width,80);
	this->addChild(yaoping,120,80);
	auto yaoping1 = Sprite::createWithSpriteFrameName("xue.png");
	yaoping1->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	yaoping1->setPosition(40+(yaoping->getContentSize().width)*2,80);
	this->addChild(yaoping1,120,81);
		auto yaoping2 = Sprite::createWithSpriteFrameName("xue.png");
	yaoping2->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	yaoping2->setPosition(40+(yaoping->getContentSize().width)*3,80);
	this->addChild(yaoping2,120,82);
		auto yaoping3 = Sprite::createWithSpriteFrameName("xue.png");
	yaoping3->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	yaoping3->setPosition(40+(yaoping->getContentSize().width)*4,80);
	this->addChild(yaoping3,120,83);

		auto yaoping4 = Sprite::createWithSpriteFrameName("xue.png");
	yaoping4->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	yaoping4->setPosition(40+(yaoping->getContentSize().width)*5,80);
	this->addChild(yaoping4,120,84);

	return true;
}



void gamescene::createBullet(float)
{
    
}

void gamescene::createEnemy(EnemyType t)
{
    
}


