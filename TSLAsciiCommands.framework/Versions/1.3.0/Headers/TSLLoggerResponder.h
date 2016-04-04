//
//  TSLLoggerResponder.h
//  AgoutiCommands
//
//  Created by Brian Painter on 15/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiCommandResponderBase.h"

///
/// A simple responder that inserts every line it sees, preceded by '>', into the standard log file
///
@interface TSLLoggerResponder : TSLAsciiCommandResponderBase

/// @name Factory Methods

/// @return a TSLLoggerResponder with default configuration
+(TSLLoggerResponder *)defaultResponder;

@end
