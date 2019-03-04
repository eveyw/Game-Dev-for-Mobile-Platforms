//
//  ViewController.swift
//  dot
//
//  Created by WEN YANG on 3/4/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    // MARK: - enum
    fileprivate enum ScreenEdge: Int {
        case top = 0
        case right = 1
        case bottom = 2
        case left = 3
    }
    
    fileprivate enum GameState {
        case ready
        case playing
        case gameOver
    }
    
    // 53
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }


}

