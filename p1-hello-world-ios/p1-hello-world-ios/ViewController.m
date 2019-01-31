//
//  ViewController.m
//  p1-hello-world-ios
//
//  Created by WEN YANG on 1/26/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController
@synthesize gif;

- (void)viewDidLoad {
    [super viewDidLoad];
    gif.animationImages = [[NSArray alloc]initWithObjects:
                           [UIImage imageNamed:@"giphy.gif"],
                           [UIImage imageNamed:@"giphy.gif"],
                           
                           nil];
    [gif setAnimationRepeatCount:0];
    gif.animationDuration = 2;
    [gif startAnimating];
    // Do any additional setup after loading the view, typically from a nib.
}


@end
