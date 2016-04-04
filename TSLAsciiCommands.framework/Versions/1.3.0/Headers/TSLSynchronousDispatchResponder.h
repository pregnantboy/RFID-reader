//
//  TSLSynchronousDispatchResponder.h
//  AgoutiCommands
//
//  Created by Brian Painter on 15/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TSLAsciiCommandResponder.h"
#import "TSLAsciiCommandResponseNotifying.h"

///
/// This is a special TSLAsciiCommandResponder that is inserted into the responder chain to handle synchronous commands
///
/// This responder uses its `synchronousCommandDelegate` to find the currently executing synchronous command. If such a command exists then this responder forwards calls to `processReceivedLine:moreLinesAvailable:` to the synchronous command's `synchronousCommandResponder`
///
@interface TSLSynchronousDispatchResponder : NSObject <TSLAsciiCommandResponder, TSLAsciiCommandResponseNotifying>

@end
