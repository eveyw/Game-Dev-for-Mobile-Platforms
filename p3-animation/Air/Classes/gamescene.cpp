#include "gamescene.h"
#include "commData.h"
#include "Enemy.h"
#include "Bullet.h"


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

void gamescene::update(float dt)
{
    auto bg1 = this->getChildByTag(1);
    auto bg2 = this->getChildByTag(2);
    bg1->setPositionY(bg1->getPositionY()-2);
    bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
    if (bg2->getPositionY() <= 0)
    {
        bg1->setPositionY(0);
    }
    Vector<Bullet*> vRemoveBul;
    for(auto bul: m_vBullets)
    {
        bul->setPositionY(bul->getPositionY() + 4);
        if(bul->getPositionY() >= (visibleSize.height + bul->getContentSize().height/2))
        {
            vRemoveBul.pushBack(bul);
            this->removeChild(bul);
        }
    }
    
    Vector<Enemy*> vEnemys;
    auto hero3 = this->getChildByTag(3);
    Vector<Bullet*> vRemoveEneBul;
    /*if(is_undead){*/
    for(auto bul1:enem_vBulets)
    {
        bul1->setPosition(bul1->getPositionX()-3,bul1->getPositionY()-3);
        if(this->m_planeActive&& isjia ==false&&hero3->getBoundingBox().intersectsRect(bul1->getBoundingBox()))
        {
            if(hero_blood>1)
            {
                herohit();
            }
            if(hero_blood<=1){heroBlowup();}
        }
        
        if(bul1->getPositionY() <= 0)
        {    vRemoveEneBul.pushBack(bul1);
            this->removeChild(bul1);
        }
        vRemoveEneBul.pushBack(bul1);
        this->removeChild(bul1);
    }
    for(auto bul2:enem2_vBulets)
    {
        bul2->setPosition(bul2->getPositionX()+3,bul2->getPositionY()-3);
        if(this->m_planeActive&&isjia==false &&hero3->getBoundingBox().intersectsRect(bul2->getBoundingBox()))
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
            if(hero_blood>1)
            {
                herohit();
            }
            if(hero_blood<=1){heroBlowup();}
        }
        else if(bul2->getPositionY() <= 0)
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
        }
    }
    
    for(auto bul2:enem3_vBulets)
    {
        bul2->setPosition(bul2->getPositionX()-1.5,bul2->getPositionY()-3);
        if(this->m_planeActive&& isjia==false&&hero3->getBoundingBox().intersectsRect(bul2->getBoundingBox()))
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
            if(hero_blood>1)
            {
                herohit();
            }
            if(hero_blood<=1){heroBlowup();}
        }
        if(bul2->getPositionY() <= 0)
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
        }
    }
    
    for(auto bul2:enem4_vBulets)
    {
        bul2->setPosition(bul2->getPositionX()+1.5,bul2->getPositionY()-3);
        if(this->m_planeActive&& isjia==false&&hero3->getBoundingBox().intersectsRect(bul2->getBoundingBox()))
        {vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
            if(hero_blood>1)
            {
                herohit();
            }
            if(hero_blood<=1){heroBlowup();}
        }
        else if(bul2->getPositionY() <= 0)
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
        }
    }
    for(auto bul2:enem5_vBulets)
    {
        bul2->setPosition(bul2->getPositionX(),bul2->getPositionY()-3);
        if(this->m_planeActive&& isjia==false&&hero3->getBoundingBox().intersectsRect(bul2->getBoundingBox()))
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
            if(hero_blood>1)
            {
                herohit();
            }
            if(hero_blood<=1){heroBlowup();}
        }
        else if(bul2->getPositionY() <= 0)
        {    vRemoveEneBul.pushBack(bul2);
            this->removeChild(bul2);
        }
    }
    
    for(auto Enemy2:this->m_vEnemys)
    {
        Enemy2->setPositionY(Enemy2->getPositionY()-Enemy2->getEnemyStep());
        if(Enemy2->getPositionY() <=-Enemy2->getContentSize().height/2)
        {if(Enemy2->m_type ==BIGENEMY){is_undead = false;}
            vEnemys.pushBack(Enemy2);
            this->removeChild(Enemy2);
        }
    }
    
    for(auto ene :m_vEnemys)
    {
        for(auto bul: m_vBullets)
        {
            if(ene->getBoundingBox().intersectsRect(bul->getBoundingBox()))
            {
                this ->removeChild(bul);
                vRemoveBul.pushBack(bul);
                if(ene->getHp() <= 1)
                {
                    ene->EnemyBlow();
                    if(ene->m_type == BIGENEMY)
                    {
                        is_undead = false;
                    }
                    this->m_EnergyNumber += ene->getScore(ene->m_type)/3;
                    updateEnergyLabel();
                    this->SumScore += ene->getScore(ene->m_type);
                    auto lbl = (Label *) this->getChildByTag(90);
                    lbl->setString(StringUtils::format("%d", this->SumScore));
                    int s;
                    if( (s =this->SumScore )> 100 * LevelCount)
                    {
                        LevelCount++;
                        Enemy::incrementSpeed();
                        if(LevelCount <= 4)
                        {
                            L_LevelUp();
                        }
                    }
                    vEnemys.pushBack(ene);
                }
                else
                {
                    ene->EnemyHit();
                }
            }
        }
    }
    
    
    for(auto Energy : m_vLaser)
    {
        for(auto ene : m_vEnemys)
        {
            if ( Energy->getBoundingBox().intersectsRect(ene->getBoundingBox()))
            {
                ene->EnemyBlow();
                this->SumScore += ene->getScore(ene->m_type);
            }
        }
        Energy->BulletBlow();
        auto lbl = (Label *) this->getChildByTag(90);
        lbl->setString(StringUtils::format("%d", this->SumScore));
        vRemoveBul.pushBack(Energy);
    }
    this->m_vLaser.clear();
    
    
    Vector<Ufo *> vUfos;
    for(auto ufo : this->m_vUfos)
    {
        auto hero = this->getChildByTag(3);
        if (ufo->getBoundingBox().intersectsRect(hero->getBoundingBox()))
        {
            
            if(ufo->m_type == SUPERBULLET)
            {
                SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
                SimpleAudioEngine::getInstance()->playEffect("get_double_laser.mp3");
                this->m_isDoubleBullet = true;
                this->m_doubleBulletNumber = 30;
            }
            else if(ufo->m_type == SUPERBOMB)
            {
                SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
                SimpleAudioEngine::getInstance()->playEffect("get_bomb.mp3");
                //this->m_isSuperBomb = true;
                if (m_SuperBombNumber < 5)
                {
                    this->m_SuperBombNumber ++;
                    auto lbl = (Label *)this->getChildByTag(91);
                    lbl->setString(StringUtils::format("X%d", this->m_SuperBombNumber));
                }
                updateBombLabel();
            }
            else if(ufo->m_type == SUPERLIGHT)
            {
                this->m_planeActive = false;
                this->isjia = true;
            }
            else if (ufo->m_type == BLOOD)
            {
                hero_blood = hero_blood+1;
                bloodupdate();
            }
            this->removeChild(ufo);
            vUfos.pushBack(ufo);
        }
    }
    
   
    for(auto enem:m_vEnemys)
    {
        auto hero1 =this->getChildByTag(3);
        if(this->m_planeActive && enem->getBoundingBox().intersectsRect(hero1->getBoundingBox()))
        {
            vEnemys.pushBack(enem);
            enem->EnemyBlow();
            this->SumScore += enem->getScore(enem->m_type);
            auto lblScoreNumber = (Label *)this->getChildByTag(90);
            lblScoreNumber->setString(StringUtils::format("%d", this->SumScore));
            if(isjia ==false)
            {
                if(this->hero_blood>1)
                {
                    herohit();
                }
                if(hero_blood<=1)
                {
                    heroBlowup();
                    SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
                    SimpleAudioEngine::getInstance()->playEffect("game_over.mp3");
                }
            }
        }
    }
    
    if(m_isbomb)
    {
        auto bomb5 = this->getChildByTag(22);
        auto hero = this->getChildByTag(3);
        bomb5->setVisible(true);
        bomb5->setPositionY(bomb5->getPositionY()+4);
        if(bomb5->getPositionY() >=(visibleSize.height-visibleSize.height/4 ))
        {
            
            for (auto enemy : this->m_vEnemys)
            {
                enemy->EnemyBlow();
                this->is_undead = false;
                this->SumScore += enemy->getScore(enemy->m_type);
            }
            this->m_vEnemys.clear();
            auto lblScoreNumber = (Label *)this->getChildByTag(90);
            lblScoreNumber->setString(StringUtils::format("%d", this->SumScore));
            bomb5->setPosition(hero->getPosition());
            bomb5->setVisible(false);
            m_isbomb = false;
            
        }
        
    }
    
    if(isjia)
    {
        delaytime++;
    }
    if(delaytime >= 300)
    {
        this->m_planeActive = true;
        delaytime = 0;
        isjia = false;
    }
    
    for(auto bu: vRemoveBul)
    {
        m_vBullets.eraseObject(bu);
    }
    vRemoveBul.clear();
    
    for(auto en:vEnemys)
    {
        m_vEnemys.eraseObject(en);
    }
    vEnemys.clear();
    
    for(auto uf : vUfos)
    {
        m_vUfos.eraseObject(uf);
    }
    vUfos.clear();
}

void gamescene::createBullet(float)
{
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
    SimpleAudioEngine::getInstance()->playEffect("button.mp3");
    auto hero = this ->getChildByTag(3);
    if (this->m_isDoubleBullet)
    {
        auto leftBullet = Bullet::create(DOUBLEBULLET);
        leftBullet->setPosition(hero->getPosition()+ Point(-hero->getContentSize().width/3, 0));
        this->addChild(leftBullet, 5);
        this->m_vBullets.pushBack(leftBullet);
        
        auto rightBullet = Bullet::create(DOUBLEBULLET);
        rightBullet->setPosition(hero->getPosition()+ Point(hero->getContentSize().width/3, 0));
        this->addChild(rightBullet, 6);
        this->m_vBullets.pushBack(rightBullet);
        
        this->m_doubleBulletNumber--;
        if (this->m_doubleBulletNumber <= 0)
        {
            this->m_isDoubleBullet = false;
        }
    }
    else
    {
        auto bul = Bullet::create(SINGLEBULLET);
        bul->setPosition(hero->getPositionX(),hero->getPositionY()+ hero->getContentSize().height/2);
        this->addChild(bul,1,4);
        m_vBullets.pushBack(bul);
    }
}

void gamescene::createLaser(float)
{
    if(this->m_isLaser)
    {
        auto hero = getChildByTag(3);
        auto las = Bullet::create(LASER);
        las->setAnchorPoint(Point(0.5,0.5));
        las->setPosition(hero->getPositionX()+hero->getContentSize().width/2,hero->getPositionY()+ hero->getContentSize().height/2+las->getContentSize().height/4);
        this->addChild(las,0,7);
        m_vLaser.pushBack(las);
        this->m_isLaser = false;
    }
}

void gamescene::createEnemy_bul(float)
{
    auto enem = this->getChildByTag(52);
    if(this->is_undead)
    {
        auto enembul = Bullet::create(BOSS_BULLET);
        enembul->setPosition(enem->getPositionX(), enem->getPositionY()-enem->getContentSize().height/3);
        enem_vBulets.pushBack(enembul);
        this->addChild(enembul, 99, 53);
        auto enembul2 = Bullet::create(BOSS_BULLET);
        enembul2->setPosition(enem->getPositionX(), enem->getPositionY()-enem->getContentSize().height/3);
        enem2_vBulets.pushBack(enembul2);
        this->addChild(enembul2, 99, 53);
        auto enembul3 = Bullet::create(BOSS_BULLET);
        enembul3->setPosition(enem->getPositionX(), enem->getPositionY()-enem->getContentSize().height/3);
        enem3_vBulets.pushBack(enembul3);
        this->addChild(enembul3, 99, 53);
        auto enembul4 = Bullet::create(BOSS_BULLET);
        enembul4->setPosition(enem->getPositionX(), enem->getPositionY()-enem->getContentSize().height/3);
        enem4_vBulets.pushBack(enembul4);
        this->addChild(enembul4, 99, 53);
        auto enembul5 = Bullet::create(BOSS_BULLET);
        enembul5->setPosition(enem->getPositionX(), enem->getPositionY()-enem->getContentSize().height/3);
        enem5_vBulets.pushBack(enembul5);
        this->addChild(enembul5, 99, 53);
    }
}

void gamescene::createEnemy(EnemyType t)
{
    auto ene = Enemy::create(t);
    float y = visibleSize.height + (ene->getContentSize().height/2);
    auto minX = ene->getContentSize().width/2;
    auto maxX = visibleSize.width- minX;
    
    float x = rand()%(int)(maxX - minX) + minX;
    ene->setPosition(x,y);
    switch (t)
    {
        case SMALLENEMY:
            this->addChild(ene,5,50);
            break;
        case MIDENEMY:
            this->addChild(ene,5,51);
            break;
        case BIGENEMY:
            this->addChild(ene,5,52);
            break;
        default:
            break;
    }
    this-> m_vEnemys.pushBack(ene);
    
}

void gamescene::createUfos(UfoType te)
{
    auto ufo = Ufo::create(te);
    auto minX = ufo->getContentSize().width/2;
    auto maxX = visibleSize.width - minX;
    
    float x = rand()%(int)(maxX - minX) + minX;
    ufo->setPosition(x,visibleSize.height + ufo->getContentSize().height/2);
    
    this->addChild(ufo);
    m_vUfos.pushBack(ufo);
    
    auto mov1 =MoveBy::create(1.5f,Point(0,-400));
    auto mov2 =MoveBy::create(1.5f,Point(0,400));
    auto mov3 =MoveTo::create(2.4f,Point(x,-ufo->getContentSize().height/2));
    
    auto call = CallFuncN::create([](Node* node)
                                  {
                                      node->removeFromParentAndCleanup(true);
                                  });
    ufo->runAction(Sequence::create(mov1,mov2,mov3,call,NULL));
}
