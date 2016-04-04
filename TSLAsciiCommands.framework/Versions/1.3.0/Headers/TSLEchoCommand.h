//
//  TSLEchoCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 02/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// A command to configure and query the reader's echo options
///
/// Note: This command has little use in an Application - the echoed command will be ignored but consumes bandwidth.
/// But, the command may be useful in a reader configuration application
///
@interface TSLEchoCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

/// When set to `TSL_TriState_YES` the reader will echo all commands
@property (nonatomic, readwrite) TSL_TriState echoEnabled;


///
/// Returns a synchronous TSLEchoCommand
///
+(TSLEchoCommand *)synchronousCommand;


@end
