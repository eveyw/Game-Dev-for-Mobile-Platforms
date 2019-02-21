//
//  MenuLayer.m
//  Animation
//
//  Created by WEN YANG on 2/20/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

-(id) init
{
    if( (self=[super init]) ) {
        CGSize winSize = [[CCDirector sharedDirector] winSize];
        CCMenuItemFont* startItem = [CCMenuItemFont itemWithString:@"Game Start"
                                                            target:self selector:@selector(startGame:)];
        startItem.position=ccp(winSize.width/2, winSize.height*0.6);
        CCMenuItemFont* settingItem = [CCMenuItemFont itemWithString:@"Game Setting"
                                                              target:self selector:@selector(setting:)];
        settingItem.position=ccp(winSize.width/2, winSize.height*0.4);
        CCMenu* menu = [CCMenu menuWithItems:startItem,settingItem, nil];
        menu.position = CGPointZero;
        [self addChild:menu];
    }
    return self;
}

-(void) startGame:(id)sender{
    CCTransitionSlideInL* transitionScene = [CCTransitionSlideInL
                                             transitionWithDuration:2.0 scene:[PreloadLayer scene]];
    [[CCDirector sharedDirector] replaceScene:transitionScene];
}

-(void) setting:(id)sender{
    CCTransitionSlideInL* transitionScene = [CCTransitionSlideInL
                                             transitionWithDuration:2.0 scene:[SettingLayer scene]];
    [[CCDirector sharedDirector] replaceScene:transitionScene];
}
