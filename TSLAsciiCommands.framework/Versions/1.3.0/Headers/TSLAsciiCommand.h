//
//  TSLAsciiCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 18/07/2012.
//  Copyright (c) 2012 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

// Forward protocol references
@protocol TSLAsciiCommandResponder;


///
/// Defines an ASCII command that can be performed on any device supporting the TSL ASCII 2.0 Protocol
///
/// A TSLAsciiCommand can be executed using any object that implements the TSLAsciiCommandExecuting protocol.
/// The command can be executed either asynchronously or synchronously (by setting synchronousCommandResponder prior to execution).
/// Synchronous commands prevent the issue of subsequent commands until the command's response has been received.
///
@protocol TSLAsciiCommand <NSObject>

///@name Full Command and Name

/// Gets the Ascii command line (including terminators) to be sent to the device
@property (nonatomic, readonly) NSString *commandLine;

/// Gets the Ascii command identifier e.g. '.vr' or '.da'
@property (nonatomic, readonly) NSString *commandName;


///@name Response Handling

/// Set this to a valid TSLAsciiCommandResponder to make the command a synchronous command
@property (weak, nonatomic, readwrite) id<TSLAsciiCommandResponder> synchronousCommandResponder;

/// Set this to the maximum time to wait for this command to complete when invoked synchronously
@property (nonatomic, readonly) NSTimeInterval maxSynchronousWaitTime;

@end
