//
//  TSLSleepTimeoutCommand_Tests.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 22/08/2014.
//  Copyright (c) 2014 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <TSLAsciiCommands/TSLAsciiCommands.h>


///
/// A command to change the reader sleep timeout
///
@interface TSLSleepTimeoutCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

///
///@return the minimum valid value for the duration parameter
///
+(int)minimumDuration;

///
///@return the maximum valid value for the duration parameter
///
+(int)maximumDuration;

/// The duration for the sleep timeout in the range (minimumDuration - maximumDuration)
@property (nonatomic, readwrite) int duration;

///
/// Returns a synchronous TSLSleepTimeoutCommand
///
+(TSLSleepTimeoutCommand *)synchronousCommand;

///
/// Returns a synchronous TSLSleepTimeoutCommand configured for the given duration
/// @param duration The duration for the sleep timeout (15-999) in seconds
///
+(TSLSleepTimeoutCommand *)synchronousCommandWithDuration:(int)duration;

@end
