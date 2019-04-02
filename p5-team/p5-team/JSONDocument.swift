//
//  JSONDocument.swift
//  p5-team
//
//  Created by WEN YANG on 4/2/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

import Foundation

typealias ResourcesByTypeAndId = [String : ResourcesById]
typealias ResourcesById        = [String : JSONAPIResource]


open class JSONAPIDocument: JSONPrinter {
    
    open var data: [JSONAPIResource] = []
    open var links: [String:URL] = [:]
    open var included: [JSONAPIResource] = []
    open var url: URL?
    open var meta: Dictionary<String,Any>?
    open var errors: [JSONAPIError] = []
    
    public convenience init(_ json: NSDictionary) {
        self.init(json as! [String:Any])
    }
    
    public convenience init(_ json: [String:Any]) {
        self.init()
        for object in normalizeJSONAPIObjectToArray(json["data"]) {
            data.append(JSONAPIResource(object as NSDictionary, parentDocument: self, loaded: .NotLoaded))
        }
        
        for object in normalizeJSONAPIObjectToArray(json["included"]) {
            included.append(JSONAPIResource(object,parentDocument: self))
        }
        
        if let strings = json["links"] as? [String:String] {
            for (key, value) in strings {
                links[key] = URL(string: value)!
                if key == "self" {
                    url = URL(string: value)!
                }
            }
        }
        
        for object in normalizeJSONAPIObjectToArray(json["errors"]) {
            errors.append(JSONAPIError(object))
        }
        
        if let metadata = json["meta"] as? Dictionary<String,Any> {
            meta = metadata
        }
        
}
    
