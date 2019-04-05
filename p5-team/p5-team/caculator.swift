//
//  caculator.swift
//  p5-team
//
//  Created by WEN YANG on 4/4/19.
//  Copyright © 2019 WEN YANG. All rights reserved.
//

import Foundation

class calculator
{
    private var accumulator = 0.0
    private var internalProgram = [AnyObject]()

    func setOperand(operand: Double) {
        accumulator = operand
        internalProgram.append(operand as AnyObject)
    }
    
    private var operations: Dictionary<String, Operation> = [
        "π": Operation.Constant(Double.pi),
        "e": Operation.Constant(M_E),
        "±": Operation.UnaryOperation({ -$0 }),
        "√": Operation.UnaryOperation(sqrt),
        "cos": Operation.UnaryOperation(cos),
        "×": Operation.BinaryOperation({ $0 * $1 }),
        "÷": Operation.BinaryOperation({ $0 / $1 }),
        "+": Operation.BinaryOperation({ $0 + $1 }),
        "−": Operation.BinaryOperation({ $0 - $1 }),
        "=": Operation.Equals
    ]
    
    private enum operation {
        case Constant(Double)
        case UnaryOperation((Double) -> Double)
        case BinaryOperation((Double, Double) -> Double)
        case Equals
    }
    
        func performoperation(symbol: String) {
            internalProgram.append(symbol as AnyObject)
            if let operation = operations[symbol] {
                switch operation {
                case .Constant(let value):
                    accumulator = value
                case .UnaryOperation(let function):
                    accumulator = function(accumulator)
                case .BinaryOperation(let function):
                    executePendingBinaryOperation()
                    pending = PendingBinaryOperationInfo(binaryFunction: function, firstOperand: accumulator)
                case .Equals:
                    executePendingBinaryOperation()
                }
            }
    }
    
    var result: Double {
        get {
            return accumulator
        }
    }
    
}
