//
//  GameController.m
//  2048
//
//  Created by WEN YANG on 2/7/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

// #import <Foundation/Foundation.h>

#import "GameController.h"

@interface GamesRootController ()

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.navigationController.navigationBar.hidden = YES;
    self.navigationController.tabBarController.tabBar.hidden = YES;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    [self initData];
    [self initMainView];
}

-(void)initData{
    _timer = nil;
    _startTime = false;
    _time = 0.00;
    _score = 0;
    _numScore = 2;
    _startMove = NO;
    _isMove = NO;
    _sum = 0;
    _allTiled = [[NSMutableArray alloc]init];
    _isMoveClearn = NO;
    _isStopGame = NO;
    _isAudioPlay = YES;
    
    for(int row=0;row<GAME_ROWS;++row){
        for(int col=0;col<GAME_COLS;++col){
            map[row][col] = _sum;
        }
    }
}

-(void)initMainView{
    self.view.backgroundColor = GAMEBACKCOLOR;
    NSInteger sizeF    = SCREEN_Width==320?14:(SCREEN_Width==375?16:17);
    NSInteger distence = SCREEN_Width==320?20:(SCREEN_Width==375?25:30);
    
    //设置游戏主题
    UILabel *title = [[UILabel alloc]initWithFrame:makeRect(25, 15+StatuBar_Height, 120, 50)];
    title.text = @"#2048";
    title.font = [UIFont systemFontOfSize:35];
    title.textColor = [UIColor whiteColor];
    title.textAlignment = NSTextAlignmentLeft;
    [self.view addSubview: title];
    
    //分数和时间
    NSArray *array = @[@"SCORE",@"TIME"];
    NSArray *numArray = @[[NSString stringWithFormat:@"%ld",(long)_score],
                          [NSString stringWithFormat:@"%0.2lf",_time]];
    for(int i=0;i<2;++i)
    {
        //承载视图
        bringView[i] = [[UIView alloc]initWithFrame:makeRect(155+i*80, 15+StatuBar_Height, 70, 50)];
        bringView[i].backgroundColor = SCORECOLOR;
        bringView[i].layer.cornerRadius = 5;
        [self.view addSubview:bringView[i]];
        
        //分数和时间主题
        bringLabel[i] = [[UILabel alloc]initWithFrame:makeRect(0, 0, 70, 25)];
        bringLabel[i].text = array[i];
        bringLabel[i].font = [UIFont systemFontOfSize:13];
        bringLabel[i].textAlignment = NSTextAlignmentCenter;
        bringLabel[i].textColor = makeColorRgb(193, 223, 245);
        [bringView[i] addSubview:bringLabel[i]];
        
        //分数和时间
        numLabel[i] = [[UILabel alloc]initWithFrame:makeRect(0, 25, 70, 20)];
        numLabel[i].text = numArray[i];
        numLabel[i].font = [UIFont systemFontOfSize:17];
        numLabel[i].textAlignment = NSTextAlignmentCenter;
        numLabel[i].textColor = makeColorRgb(193, 223, 245);
        [bringView[i] addSubview:numLabel[i]];
    }
    
    
    
    //背景块的尺寸
    _tiledmapwidth=GAME_TILED_WIDTH*GAME_COLS+GAME_TILED_BOARD_WIDTH*(GAME_COLS+1);
    _tiledmapheight=GAME_TILED_HEIGTH*GAME_ROWS+GAME_TILED_BOARD_WIDTH*(GAME_ROWS+1);
    
    //灰色的背景
    _colorZone = [[UIView alloc]init];
    _gameX = self.view.centerX;
    _gameY = bringView[1].bottom+distence+_tiledmapheight/2;
    _colorZone.center = CGPointMake(_gameX, _gameY);
    _colorZone.bounds = makeRect(0, 0,_tiledmapwidth , _tiledmapheight);
    _colorZone.backgroundColor = GAMEBACKCOLOR;
    _colorZone.alpha = 1;
    [self.view addSubview:_colorZone];
    
    //重新调整标题、时间和分数的位置
    title.left = _colorZone.left+8;
    bringView[1].right = _colorZone.right-8;
    bringView[0].right = bringView[1].left-10;
    
    //设置背景块
    for(int row=0;row<GAME_ROWS;++row)
    {
        for(int col=0;col<GAME_COLS;++col)
        {
            UIView *tiledView = [[UIView alloc]init];
            tiledView.frame = makeRect(GAME_TILED_WIDTH*col+GAME_TILED_BOARD_WIDTH*(col+1)
                                       , GAME_TILED_HEIGTH*row+GAME_TILED_BOARD_WIDTH*(row+1)
                                       , GAME_TILED_WIDTH, GAME_TILED_HEIGTH);
            tiledView.backgroundColor = TILEDCOLOR;
            [_colorZone addSubview:tiledView];
            
        }
    }
    
    //下面的三个按钮
    NSArray *btnNoArr = @[@"Close",@"Again", @"Play"];
    NSArray *btnSeArr = @[@"Open" ,@"Again", @"Stop"];
    NSArray *labArr   = @[@"Power"  ,@"Start Again",@"Audio"];
    
    for(int i=0;i<3;++i)
    {
        
        UIButton *playPauseBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        playPauseBtn.bounds = makeRect(0,0, BTN_SIZE, BTN_SIZE);
        playPauseBtn.top = _colorZone.bottom + 25;
        playPauseBtn.tag = 10+i;
        playPauseBtn.backgroundColor = SCORECOLOR;
        playPauseBtn.layer.cornerRadius = BTN_SIZE/2;
        [playPauseBtn setTitle:btnNoArr[i] forState:UIControlStateNormal];
        [playPauseBtn setTitle:btnSeArr[i] forState:UIControlStateSelected];
        [playPauseBtn setTintColor:[UIColor whiteColor]];
        playPauseBtn.selected = NO;
        [playPauseBtn addTarget:self action:@selector(buttonPressed:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:playPauseBtn];
        
        
        UILabel *playPauseLab = [[UILabel alloc]init];
        playPauseLab.bounds = makeRect(0,0, LAB_WIDTH, 40);
        playPauseLab.top = playPauseBtn.bottom+5;
        playPauseLab.text = labArr[i];
        playPauseLab.font = makeFont(sizeF);
        playPauseLab.textColor = [UIColor whiteColor];
        playPauseLab.textAlignment = NSTextAlignmentCenter;
        [self.view addSubview:playPauseLab];
        
        if(i==0)playPauseBtn.left = _colorZone.left+8;
        else if(i==1)playPauseBtn.centerX = self.view.centerX;
        else playPauseBtn.right = _colorZone.right-8;
        playPauseLab.centerX = playPauseBtn.centerX;
        
    }
    
    
    
    [self newMoveTiled];
}

//一个新的可移动的块
-(void)newMoveTiled{
    MoveTieldView *tiled = [[MoveTieldView alloc]initWithFrame:
                            makeRect(0, 0, GAME_TILED_WIDTH, GAME_TILED_HEIGTH)];
    
    int freeCount=16-(int)[_allTiled count];
    if(!freeCount){
        return;
    }
    int num= arc4random() % freeCount;
    int row=0;
    int col=0;
    int count=-1;
    bool find=false;
    
    for(;row<GAME_ROWS;++row)
    {
        for(col=0;col<GAME_COLS;++col)
        {
            if(map[row][col]==0)
            {
                ++count;
                if(count==num)
                {
                    find=true;
                    break;
                }
            }
        }
        if(find)
        {
            break;
        }
    }
    
    [_colorZone addSubview:tiled];
    [tiled showAtx:row showAty:col];
    [_allTiled addObject:tiled];
    map[row][col] = ++_sum;
}


//确认滑动开始
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    if(_isStopGame == NO){
        _startMove = YES;
    }
    else{
        _startMove = NO;
    }
    
}

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

-(BOOL)isGameOver
{
    bool isGameOver=true;
    int freeCount=16-(int)[_allTiled count];
    if(freeCount!=0)
    {
        isGameOver=false;
    }
    if(freeCount==0)
    {
        for(int r=0;r<GAME_ROWS;++r)
        {
            for(int c=0;c<GAME_COLS;++c)
            {
                MoveTieldView *tiled = [_allTiled objectAtIndex:(map[r][c]-1)];
                NSInteger num    = tiled.number;
                NSInteger objNum = 0;
                if(r+1<GAME_ROWS)
                {
                    objNum=
                    ((MoveTieldView *)[_allTiled objectAtIndex:(map[r+1][c]-1)]).number;
                    if(num==objNum)
                    {
                        isGameOver=false;
                    }
                }
                
                if(r-1>=0)
                {
                    objNum= ((MoveTieldView *)[_allTiled objectAtIndex:(map[r-1][c]-1)]).number;
                    if(num==objNum)
                    {
                        isGameOver=false;
                    }
                }
                if(c-1>=0)
                {
                    objNum= ((MoveTieldView *)[_allTiled objectAtIndex:(map[r][c-1]-1)]).number;
                    if(num==objNum)
                    {
                        isGameOver=false;
                    }
                }
                
                if(c+1<GAME_COLS)
                {
                    objNum= ((MoveTieldView *)[_allTiled objectAtIndex:(map[r][c+1]-1)]).number;
                    if(num==objNum)
                    {
                        isGameOver=false;;
                    }
                }
                if(!isGameOver)
                {
                    break;
                }
            }
            if(!isGameOver)
            {
                break;
            }
        }
    }
    return isGameOver;
}


@end

