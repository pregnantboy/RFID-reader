//
//  TSLExecuteAutorunFileCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 07/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// A command to execute the Autorun file
///
@interface TSLExecuteAutorunFileCommand : TSLAsciiSelfResponderCommandBase

///
/// Returns a synchronous TSLExecuteAutorunFileCommand
///
+(TSLExecuteAutorunFileCommand *)synchronousCommand;

@end
