//#ifndef __TSLAsciiCommandExecuting_h__
//#define __TSLAsciiCommandExecuting_h__
//
//  TSLAsciiCommandExecuting.h
//  AgoutiCommands
//
//  Created by Brian Painter on 18/07/2012.
//  Copyright (c) 2012 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol TSLAsciiCommand;
@protocol TSLAsciiCommandResponder;

///
/// Defines the responsibilities of classes that can execute and respond to a TSLAsciiCommand
///
/// Responses to an executed TSLAsciiCommand (see `executeCommand:`) are handled through a responder chain (see `responderChain`)
/// The responder chain is an ordered list of TSLAsciiCommandResponder that is traversed from the first responder added to the last.
/// Each correctly terminated response line that has been received is passed to the TSLAsciiCommandResponder's processReceivedLine: method.
/// If a responder returns YES from the processReceivedLine method then the traversal ends otherwise it continues until all responders have been
/// visited.
///
@protocol TSLAsciiCommandExecuting <NSObject>


/// @name Executing commands

///
/// Execute the given command.
///
/// Command execution is asynchronous unless the command has a (non-nil) synchronousCommandResponder then
/// the command will be executed synchronously. Synchronous behaviour requires prior call to addSynchronousResponder.
///
/// *Warning:* derived classes must call the base implementation to ensure synchronous commands work correctly
///
/// @param command The command to be executed
///
-(void) executeCommand:(id<TSLAsciiCommand>)command;


/// @name Managing the responder chain

/// The chain of TSLAsciiCommandResponders
@property (nonatomic, readonly) id<NSFastEnumeration> responderChain;


///
/// Add a responder to the responder chain
///
/// @param responder The responder to add to the chain
///
-(void) addResponder:(id<TSLAsciiCommandResponder>)responder;

///
/// Remove a responder from the responder chain
///
/// @param responder The responder to remove from the chain
///
-(void) removeResponder:(id<TSLAsciiCommandResponder>)responder;

///
/// Add the synchronous responder into the chain
///
/// This is a special responder that despatches responses through a command's `synchronousCommandResponder` property
/// There will only ever be one of these in the command chain
///
-(void) addSynchronousResponder;

///
/// Remove the synchronous responder from the chain
///
-(void) removeSynchronousResponder;

///
/// Clear all responders from the responder chain
///
-(void) clearResponders;



@end
//#endif