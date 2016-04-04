//
//  TSLSwitchDoublePressCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 04/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// A command to programmatically double press the device switch
///
@interface TSLSwitchDoublePressCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

///
///@return the maximum valid value for the duration parameter
///
+(int)maximumDuration;

/// The duration for the switch press action (0-99)
/// Non-zero values are the time in seconds that the switch press will be maintained
/// Zero is an infinite press - use the TSLAbort command to release the switch
@property (nonatomic, readwrite) int duration;

///
/// Returns a synchronous TSLSwitchDoublePressCommand
///
+(TSLSwitchDoublePressCommand *)synchronousCommand;

///
/// Returns a synchronous TSLSwitchDoublePressCommand configured for the given duration
/// @param duration The duration for the switch press action (0-99)
///
+(TSLSwitchDoublePressCommand *)synchronousCommandWithDuration:(int)duration;

@end
