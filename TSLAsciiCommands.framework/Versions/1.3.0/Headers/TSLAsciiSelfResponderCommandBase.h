//
//  TSLAsciiSelfResponderCommandBase.h
//  AgoutiCommands
//
//  Created by Brian Painter on 19/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiCommandLibraryResponderBase.h"
#import "TSLAsciiCommand.h"


///
/// A base class for commands that are their own responders - this simplifies synchronous command creation
///
@interface TSLAsciiSelfResponderCommandBase : TSLAsciiCommandLibraryResponderBase <TSLAsciiCommand>

/// @name Initialisers

///
/// Initialise with the given commandName and responder
///
/// Designated initialiser
///
/// This will create a synchronous command
///
/// @param name The command name e.g. `'.iv'` for Inventory
/// @param responder The object to handle synchronous responses for this command
///
-(id)initWithCommandName:(NSString *)name responder:(id<TSLAsciiCommandResponder>) responder;


///
/// Initialise with a TSLAsciiCommandResponder to create a synchronous command
///
/// @param responder The object to handle synchronous responses for this command
///
-(id)initWithResponder:(id<TSLAsciiCommandResponder>)responder;


/// @name Factory Methods

///
/// Returns a synchronous command with self as responder
///
+(TSLAsciiSelfResponderCommandBase *)synchronousCommand;


@end
