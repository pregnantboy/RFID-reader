//
//  TSLSwitchSinglePressUserActionCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 04/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to configure the user-defined, single-press switch action
///
@interface TSLSwitchSinglePressUserActionCommand : TSLAsciiSelfResponderCommandBase

///
/// The ASCII command to be executed when the switch is pressed as an NSString
///
/// Set this and actionCommand to nil (the default) to read the current action string
/// This property can be set explicitly but is always set when the actionCommand is set
@property (nonatomic, readwrite) NSString *value;

/// The command to be executed when the switch is pressed as an id<TSLAsciiCommand>
///
/// Setting this will override any value that is in the value property
/// This property is 'set-only' and is not affected by the reader's response
@property (nonatomic, readwrite) id<TSLAsciiCommand> actionCommand;

///
/// Returns a synchronous TSLSwitchSinglePressUserActionCommand
///
+(TSLSwitchSinglePressUserActionCommand *)synchronousCommand;

@end
