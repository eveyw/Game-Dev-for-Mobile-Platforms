//
//  ViewController.swift
//  length-convert
//
//  Created by WEN YANG on 4/5/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    @IBOutlet weak var textInches: UILabel!
    @IBOutlet weak var textCentimeters: UILabel!
    
    @IBOutlet weak var textFeet: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    @IBAction func textFieldOne(_ sender: UITextField) {
        sender.resignFirstResponder()
        
    }
        
    @IBAction func calcButton(_ sender: Any) {
        
        let feet = textFeet.text!
        let reuslt1: Double = Double(feet)! * 12
        print(reuslt1)
        textInches.text = String(reuslt1)
        
        let reuslt2: Double = Double(feet)! * 30.48
        print(reuslt2)
        textCentimeters.text = String(reuslt2)
        
    }
    
    /*
    @IBAction func cenButton(_ sender: UIButton) {
        
        let feet = textFeet.text!
        let reuslt: Double = Double(feet)! * 30.48
        print(reuslt)
        textCentimeters.text = String(reuslt)
    }
 */
    @IBAction func clearButton(_ sender: Any) {
        
        let reuslt1: Double = 0.0
        print(reuslt1)
        textInches.text = String(reuslt1)
        
        let reuslt2: Double = 0.0
        print(reuslt2)
        textCentimeters.text = String(reuslt2)
        
    }

}

