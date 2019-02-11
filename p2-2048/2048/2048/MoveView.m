//
//  MoveView.m
//  2048
//
//  Created by WEN YANG on 2/8/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//


#import "MoveView.h"
@implementation MoveTieldView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self){
        [self initMainView];
    }
    return self;
}


-(void)initMainView
{
    int n = arc4random() % 10;
    _number = (n == 5)?4:2;
    
    
    _label = [[UILabel alloc]init];
    _label.frame = makeRect(0, 0, GAME_TILED_WIDTH, GAME_TILED_HEIGTH);
    _label.tag = 10;
    [_label setText:[NSString stringWithFormat:@"%d",_number]];
    _label.font = [UIFont boldSystemFontOfSize:30];
    _label.textColor = TILEDColor;
    _label.textAlignment = NSTextAlignmentCenter;
    [self addSubview:_label];
    
    self.backgroundColor = [UIColor setTiledColor:_number];
    self.tag = 110;
    
    if(_number>=1000){
        _label.font = [UIFont boldSystemFontOfSize:20];
    }
    
}


-(void)showAtx:(int)r
       showAty:(int)w{
    
    _row = r;
    _col = w;
    
    self.frame = makeRect(_col*GAME_TILED_WIDTH+GAME_TILED_BOARD_WIDTH*(_col+1),
                          _row*GAME_TILED_HEIGTH+GAME_TILED_BOARD_WIDTH*(_row+1),
                          GAME_TILED_WIDTH,GAME_TILED_HEIGTH);
    [self changeBig];
}


-(void)moveTox:(int)r
       moveToy:(int)c{
    _row = r;
    _col = c;
    [UIView animateWithDuration:0.1f animations:^
     {
         self.frame = makeRect(_col*GAME_TILED_WIDTH+GAME_TILED_BOARD_WIDTH*(_col+1),
                               _row*GAME_TILED_HEIGTH+GAME_TILED_BOARD_WIDTH*(_row+1),
                               GAME_TILED_WIDTH,GAME_TILED_HEIGTH);
     } completion:nil];
}


-(void)doubleNumber{
    
    _number *=2;
    _label.text = [NSString stringWithFormat:@"%d",_number];
    [self changeBig];
    
    int num=0;
    int k=_number;
    while(k!=1){
        num++;
        k=k/2;
    }
    self.backgroundColor = [UIColor setTiledColor:_number];
    _label.textColor = _number<=4?TILEDColor:makeColorRgb(248, 245, 245);
    if(num>=10){
        _label.font = [UIFont boldSystemFontOfSize:20];
    }
}



@end
