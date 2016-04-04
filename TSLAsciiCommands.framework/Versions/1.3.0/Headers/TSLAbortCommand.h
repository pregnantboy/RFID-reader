//
//  TSLAbortCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 02/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to terminate the current command and any pending commands. It also stops any software switch presses that are in progress.
///
@interface TSLAbortCommand : TSLAsciiSelfResponderCommandBase

/// @name Factory Methods

///
/// Returns a synchronous TSLAbortCommand
///
+(TSLAbortCommand *)synchronousCommand;

@end
