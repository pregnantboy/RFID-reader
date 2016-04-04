//
//  TSLSleepCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 19/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to send the reader to sleep as soon as it has responded to this command
///
/// *Note:* this will disconnect the reader from the iOS device
///
@interface TSLSleepCommand : TSLAsciiSelfResponderCommandBase

/// @name Factory Methods

///
/// Returns a synchronous TSLDateCommand
///
+(TSLSleepCommand *)synchronousCommand;

@end
