#include "GameOverScene.h"
#include "commData.h"

GameOverScene:: GameOverScene()
{
	highScore = 0;
}

Scene* GameOverScene::createScene(int s)
{
	auto scene = Scene::create();
	auto lay = GameOverScene::create(s);
	scene->addChild(lay);
	return scene;
}

GameOverScene* GameOverScene::create(int s)
{
	auto gma = new GameOverScene;
	if(gma && gma->init(s))
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

bool GameOverScene::init(int s)
{
	if(!Layer::init())
	{
		return false;
	}	
	auto bg = Sprite::createWithSpriteFrameName("gameover.png");
	bg->setAnchorPoint(Point::ZERO);
	this->addChild(bg);

	

	auto lblScore = Label::createWithSystemFont("SumScore:", "Arial", 24);
	lblScore->setPosition(100, 320);
	lblScore->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblScore->setColor(Color3B::RED);
	this->addChild(lblScore, 99);

	
	auto lblScoreNumber = Label::createWithBMFont("font.fnt", "0");
	lblScoreNumber->setPosition(lblScore->getContentSize().width + 110, 320);
	lblScoreNumber->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblScoreNumber->setColor(Color3B::YELLOW);
	this->addChild(lblScoreNumber, 99, 90);

	lblScoreNumber->setString(StringUtils::format("%d", s));
	auto scaleIn = ScaleTo::create(0.5f, 1.5);
	auto scaleOut = ScaleTo::create(0.5f, 1);
	lblScoreNumber->runAction(Sequence::create(scaleIn, scaleOut, NULL));


	auto lblScore0 = Label::createWithSystemFont("HighScore:", "Arial", 24);
	lblScore0->setPosition(100, 450);
	lblScore0->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lblScore0->setColor(Color3B::RED);
	this->addChild(lblScore0, 99);

	auto HighScoreNumber = Label::createWithBMFont("font.fnt", "0");
	HighScoreNumber->setPosition(lblScore0->getContentSize().width + 110, 450);
	HighScoreNumber->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	HighScoreNumber->setColor(Color3B::BLUE);
	this->addChild(HighScoreNumber, 99, 90);

	highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE", highScore);
	HighScoreNumber->setString(StringUtils::format("%d", highScore));
	

	if (highScore < s)
	{
			highScore = s;
			UserDefault::getInstance()->setIntegerForKey("HIGHSCORE", highScore);

			HighScoreNumber->setString(StringUtils::format("%d", highScore));

			auto scaleIn_1 = ScaleTo::create(0.5f, 1.5);
			auto scaleOut_1 = ScaleTo::create(0.5f, 1);
			HighScoreNumber->runAction(Sequence::create(scaleIn_1, scaleOut_1, NULL));
			SimpleAudioEngine::getInstance()->setEffectsVolume(0.3f);
			SimpleAudioEngine::getInstance()->playEffect("achievement.mp3");
	}

	auto spriteresume1 = Sprite::createWithSpriteFrameName("restart.png");
	auto spriteresume2 = Sprite::createWithSpriteFrameName("restart.png");
	auto menuItemBack = MenuItemSprite::create(spriteresume1, spriteresume2, [=](Ref *){
			auto scene = gamescene::createScene();
			Director::getInstance()->replaceScene(scene);
		});
	menuItemBack->setPosition(visibleSize.width/2-14,visibleSize.height/4);
	menuItemBack->setAnchorPoint(Point::ZERO);
	auto menu = Menu::create();
	menu->addChild(menuItemBack,99,102);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 99,103);

	auto exit1 = Sprite::createWithSpriteFrameName("tuichu.png");
	auto exit2 = Sprite::createWithSpriteFrameName("tuichu.png");
	auto menuItemExit2 = MenuItemSprite::create(exit1, exit2, [](Ref *){
		
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
			return;
			#endif

			Director::getInstance()->end();

			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
			#endif
	});
	menuItemExit2->setPosition(visibleSize.width/2,visibleSize.height/6);
	auto menu3=Menu::create();
	menu3->addChild(menuItemExit2,1,121);
	menu3->setPosition(Point::ZERO);
	this->addChild(menu3, 1,122);	


	return true;
}
