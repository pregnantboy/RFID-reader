//
//  TSLSwitchStateCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 12/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// Use macro to allow the creation of enum, parameter value and descriptive strings that are always in sync
///
#define TSL_FOR_EACH_SWITCH_STATE(select)\
\
select( TSL_SwitchState_NotSpecified = 0,   @"",           @"Not specified"  )\
select( TSL_SwitchState_Off,                @"off",        @"Off"            )\
select( TSL_SwitchState_Single,             @"single",     @"Single press"   )\
select( TSL_SwitchState_Double,             @"double",     @"Double press"   )\


///
/// The switch state types
///
#define USE_ENUM(e,p,s) e,

typedef enum { TSL_FOR_EACH_SWITCH_STATE(USE_ENUM) } TSL_SwitchState;

#undef USE_ENUM


///
/// A command to query the state of the reader's switch
///
/// The `TSL_SwitchState` enum is used to specify values for the `state` property.
///
/// After issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_SwitchState_NOT_SPECIFIED` - _the switch state is unknown_
/// - `TSL_SwitchState_Off` - _the switch is off_
/// - `TSL_SwitchState_Single` - _the switch is on by a single click_
/// - `TSL_SwitchState_Double` - _the switch is on by a double click_
///
@interface TSLSwitchStateCommand : TSLAsciiSelfResponderCommandBase

/// @name Properties

/// The last switch state received from the device
@property (nonatomic, readonly) TSL_SwitchState state;

/// @name Switch State helper methods

///
/// Parse the given parameter value string and determine the corresponding TSL_SwitchState
///
///@param value The parameter value as NSString
///@return The TSL_SwitchState corresponding to the given string. Invalid parameter values return TSL_SwitchState_NotSpecified
///
+(TSL_SwitchState)switchStateForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_SwitchState
///
///@param state The TSL_SwitchState value to be converted
///@return The command parameter value for the given TSL_SwitchState or nil if invalid
///
+(NSString *)parameterValueForSwitchState:(TSL_SwitchState)state;


///
/// Get the description corresponding to the given TSL_SwitchState
///
///@param state The TSL_SwitchState value to be converted
///@return The user friendly name for the given TSL_SwitchState or nil if invalid
///
+(NSString *)descriptionForSwitchState:(TSL_SwitchState)state;

///
/// Test the given TSL_SwitchState for validity
///
///@param state The TSL_SwitchState value to be tested
///@return YES if the given TSL_SwitchState is valid otherwise NO
///
+(BOOL)isValidSwitchState:(TSL_SwitchState)state;


/// @name Factory Methods

///
/// Returns a synchronous TSLSwitchStateCommand
///
+(TSLSwitchStateCommand *)synchronousCommand;


@end
