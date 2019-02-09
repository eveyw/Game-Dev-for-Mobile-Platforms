//
//  MoveView.h
//  2048
//
//  Created by WEN YANG on 2/8/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//


#import "GameDefine.h"


@property(nonatomic,assign)int row;
@property(nonatomic,assign)int col;

@property(nonatomic,strong)UILabel *label;
@property(nonatomic,assign)int number;


-(void)showAtx:(int)r showAty:(int)w;



-(void)moveTox:(int)r moveToy:(int)c;




-(void)doubleNumber;




@end

