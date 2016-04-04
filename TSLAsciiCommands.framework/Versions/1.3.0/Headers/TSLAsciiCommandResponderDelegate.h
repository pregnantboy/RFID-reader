//
//  TSLAsciiCommandResponderDelegate.h
//  AgoutiCommands
//
//  Created by Brian Painter on 13/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

// Forward ref
@protocol TSLAsciiCommand;

///
/// Protocol used to allow command responders access to any _synchronous_ command that is currently executing and to inform delegates that a command has finished
///
@protocol TSLAsciiCommandResponderDelegate <NSObject>

/// Responders should call this when the end of a command is detected
-(void)commandDidFinish;

/// The currently executing synchronous command or nil if none
@property (readonly) id<TSLAsciiCommand> synchronousCommand;

@end
