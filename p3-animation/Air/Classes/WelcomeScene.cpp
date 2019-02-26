#include "WelcomeScene.h"

Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto lay = WelcomeScene::create();
	scene->addChild(lay);
	return scene;
}

WelcomeScene* WelcomeScene::create()
{
	auto gma = new WelcomeScene;
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

bool WelcomeScene::init()
{
	if(!Layer::init())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("achievement.mp3");	
	SimpleAudioEngine::getInstance()->preloadEffect("enemy1_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("enemy2_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("enemy3_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("get_bomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("use_bomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("get_double_laser.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("game_over.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("button.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("bullet.mp3");
	

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm.wav", true);
	srand((unsigned int)time(0));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("juese.plist");	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tanchu.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("background.plist");	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("daoju.plist");	


	auto animation = Animation::create();
	auto loading = Sprite::createWithSpriteFrameName("begin1.png");
	loading->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(loading,1);
	int i=0;
		for (i = 0; i < 3; i++)
		{
			auto png = StringUtils::format("begin%d.png", i+1);
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));
		}
		
		animation->setDelayPerUnit(4.0f);
		animation->setLoops(-1);

				auto enter1 = Sprite::createWithSpriteFrameName("start_menu.png");
				auto enter2 = Sprite::createWithSpriteFrameName("start_menu.png");
				auto menuItementer = MenuItemSprite::create(enter1, enter2, [](Ref *){
					auto scene = gamescene::createScene();
				Director::getInstance()->replaceScene(scene);
				});	
				menuItementer->setPosition(visibleSize.width/2,visibleSize.height/5.5);
				auto menu2=Menu::create();
				menu2 ->addChild(menuItementer,1,133);
				menu2->setPosition(Point::ZERO);
				this->addChild(menu2, 1,134);	
	auto animate = Animate::create(animation);
	auto callFunc = CallFunc::create([=](){
		auto scene = gamescene::createScene();
		Director::getInstance()->replaceScene(scene);
		});
	loading->runAction(Sequence::create(animate, callFunc, NULL));
	return true;
}
