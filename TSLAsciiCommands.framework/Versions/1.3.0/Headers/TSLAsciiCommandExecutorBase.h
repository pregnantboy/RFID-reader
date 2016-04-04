//
//  TSLAsciiCommandExecutorBase.h
//  AgoutiCommands
//
//  Created by Brian Painter on 14/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TSLAsciiCommandExecuting.h"
#import "TSLAsciiCommandResponderDelegate.h"


///
/// A base class that implements the responder chain management
///
@interface TSLAsciiCommandExecutorBase : NSObject <TSLAsciiCommandExecuting, TSLAsciiCommandResponderDelegate>

/// Test for presence of the synchronous command responder in the responder chain
@property (nonatomic, readonly) BOOL hasSynchronousResponder;

@end
