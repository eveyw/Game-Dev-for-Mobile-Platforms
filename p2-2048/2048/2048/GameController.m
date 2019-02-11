//
//  GameController.m
//  2048
//
//  Created by WEN YANG on 2/7/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

// #import <Foundation/Foundation.h>

#import "GameController.h"

@interface GameController ()

// 触摸移动
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    if(!_startTime){
        _startTime = NO;
        [self startTimer];
    }
    [super touchesMoved:touches withEvent:event];
    UITouch *touch = [touches anyObject];
    
    CGPoint previousLocation = [touch previousLocationInView:self.view];
    CGPoint currentLocation = [touch locationInView:self.view];
    
    int x = currentLocation.x - previousLocation.x;
    int y = currentLocation.y - previousLocation.y;
    int ax = x>0?x:0-x;
    int ay = y>0?y:0-y;
    
    //对开始点击屏幕的位置判断是否在游戏区域
    if(currentLocation.y<(_gameY-_tiledmapheight/2) ||
       currentLocation.y>(_gameY+_tiledmapheight/2) ||
       currentLocation.x<(_gameX-_tiledmapwidth/2)||
       currentLocation.x>(_gameX+_tiledmapwidth/2)){
        _startMove = NO;
    }
    
    //移动像素大于5  则滑动成功 判断方向
    if(_startMove && ( ax>5 || ay>5)){
        _startMove = NO;
        if(ax>ay){
            if(x>0){
                _directionType = RIGHT;
            }else{
                _directionType = LEFT;
            }
        }else{
            if(y<0){
                _directionType = UP;
            }else{
                _directionType = DOWN;
            }
        }
        [self moveAllTiled:_directionType];
    }
}




@end

