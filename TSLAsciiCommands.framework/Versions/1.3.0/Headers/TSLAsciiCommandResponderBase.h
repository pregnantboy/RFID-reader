//
//  TSLAsciiCommandResponderBase.h
//  AgoutiCommands
//
//  Created by Brian Painter on 13/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TSLAsciiCommandResponder.h"
#import "TSLAsciiCommandResponseNotifying.h"


///
/// A base class that handles the responses from ASCII 2.0 commands
///
/// TSLAsciiCommandResponders are designed to operate in a Chain of Responsibility.
/// Typically each line received from the reader is passed to each responder in the chain until all responders have seen the line or a responder in the chain has indicated that it has handled the line.
///
/// This class provides default handling for the 'CS', 'PR' and 'ME' responses and will add a copy of each handled line to the `response` property
///
@interface TSLAsciiCommandResponderBase : NSObject <TSLAsciiCommandResponder, TSLAsciiCommandResponseNotifying>

/// @name Properties

/// The command to respond to - this can be nil
@property (nonatomic, readwrite) NSString *commandName;

/// YES if a 'CS:' header beginning with commandName has been seen
@property (nonatomic, readwrite) bool responseStarted;


/// @name Initialisers

///
/// Initialise with the given command name
///
/// @param name The command name e.g. `'.iv'` for Inventory
///
-(id)initWithCommandName:(NSString *)name;


/// @name Reader response processing

///
/// Each correctly terminated line from the device is passed to this method for processing
///
/// This base class provides default handling for the following responses:
///
///   CS    Sets 'responseStarted' to YES
///   PR    Populates the 'parameters' array with parameters occurring within command responses
///   ME    Populates the 'messages' array with messages occurring within command responses
///
/// @param fullLine The line to be processed
/// @param header The response line header excluding the colon e.g. `'CS'` for a command started response
/// @param value The response line following the colon e.g. `'.iv'`
/// @param moreAvailable When YES indictates there are additional lines to be processed (and will also be passed to this method)
///
/// @return Derived classes should return YES if this line should not be passed to any other responder.
///
-(BOOL)processReceivedLine:(NSString *)fullLine header:(NSString *)header value:(NSString *)value moreLinesAvailable:(BOOL) moreAvailable;


/// @name Hooking into the default processing

/// Derived classes can override this method to provide custom handling when the command response starts
///
/// *Warning:* To ensure correct operation of Block handlers the super class method must be invoked
-(void)responseDidStart;

/// Derived classes can override this method to provide custom handling upon completion of a synchronous response
///
/// *Warning:* To ensure correct operation of synchronous commands the super class method must be invoked
-(void)responseDidFinish;

/// Derived classes can override this method to provide custom handling upon completion of an asynchronous response
-(void)responseDidFinishAsync;

/// Derived classes can override this method to provide custom handling for each parameter received
///
/// @param parameter A single parameter extracted from the PR: response, excluding the '-' and trimmed of leading and trailing whitespace
/// @return Return YES if the parameter was handled
///
-(BOOL)responseDidReceiveParameter:(NSString *)parameter;


/// @name Reader response property

/// Append the given NSString to the current `response` property
///
/// @param line The line to append to the current response
///
-(void)appendToResponse:(NSString *)line;

@end
