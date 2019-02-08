//
//  GameController.h
//  2048
//
//  Created by WEN YANG on 2/7/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GameDefine.h"


typedef NS_ENUM(NSInteger, DirectionType){
    UP=0,
    DOWN,
    LEFT,
    RIGHT
};

@interface GamesRootController : UIViewController<GameOverViewDelegate>{
    
    NSInteger map[GAME_ROWS][GAME_COLS];
    
    UIView  *bringView[2];
    UILabel *bringLabel[2];
    UILabel *numLabel[2];
}

@property(nonatomic,assign)DirectionType directionType;

@property(nonatomic,assign)CGFloat gameX;
@property(nonatomic,assign)CGFloat gameY;
@property(nonatomic,assign)CGFloat tiledmapwidth;
@property(nonatomic,assign)CGFloat tiledmapheight;


@property(nonatomic,assign)BOOL    startTime;
@property(nonatomic,strong)NSTimer *timer;
@property(nonatomic,assign)CGFloat time;
@property(nonatomic,assign)NSInteger score;
@property(nonatomic,assign)NSInteger numScore;
@property(nonatomic,assign)BOOL    startMove;
@property(nonatomic,assign)BOOL    isMove;
@property(nonatomic,assign)CGFloat sum;
@property(nonatomic,assign)BOOL    isMoveClearn;
@property(nonatomic,assign)BOOL    isStopGame;
@property(nonatomic,assign)BOOL    isAudioPlay;
@property(nonatomic,strong)UIView  *colorZone;
@property(nonatomic,strong)NSMutableArray *allTiled;
@property(nonatomic,strong)GameOverView *gameOver;




@end

