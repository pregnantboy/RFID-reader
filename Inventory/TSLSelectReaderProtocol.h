//
//  TSLSelectReaderProtocol.h
//  Inventory
//
//  Created by Brian Painter on 28/08/2014.
//  Copyright (c) 2014 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol TSLSelectReaderProtocol <NSObject>

//
// The User chose the reader on the given row of the connected accessories list
// row - the chosen row or -1 if none selected
//
-(void)didSelectReaderForRow:(NSInteger)row;

@end
