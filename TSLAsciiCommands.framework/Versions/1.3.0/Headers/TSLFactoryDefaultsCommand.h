//
//  TSLFactoryDefaultsCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 02/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to reset the reader to its default configuration
///
@interface TSLFactoryDefaultsCommand : TSLAsciiSelfResponderCommandBase

/// @name Factory Methods

///
/// Returns a synchronous TSLFactoryDefaultsCommand
///
+(TSLFactoryDefaultsCommand *)synchronousCommand;

@end
