//
//  FileM.swift
//  p5-team
//
//  Created by WEN YANG on 4/1/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

import Foundation
import UIKit

var error: NSError?
let filemgr = NSFileManager.defaultManager()
let dirPaths = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)
let docsDir = dirPaths[0] as String
let filelist = filemgr.contentsOfDirectoryAtPath("\(docsDir)", error: &error)

class FileManager {
    func dirContents() -> Array<String> {
        var list: Array<String> = []
        for filename in filelist! {
            list.append("\(filename)")
        }
        return list
    }
    
    func makeDir(name: String) -> Bool {
        let newDir = docsDir.stringByAppendingPathComponent(name)
        if !filemgr.createDirectoryAtPath(newDir, withIntermediateDirectories: true, attributes: nil, error: &error) {
            return false
        } else {
            return true
        }
    }
    
    func delDir(name: String) -> Bool {
        let newDir = docsDir.stringByAppendingPathComponent(name)
        if !filemgr.removeItemAtPath(newDir, error: &error) {
            return false
        } else {
            return true
        }
    }
}
