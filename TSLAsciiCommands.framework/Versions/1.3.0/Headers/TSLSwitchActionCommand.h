//
//  TSLSwitchActionCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 20/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLTriState.h"
#import "TSLCommandParameters.h"
#import "TSLAsciiSelfResponderCommandBase.h"

///
/// Use macro to allow the creation of enum, command parameter and descriptive strings that are always in sync
///
/// off/rd/wr/inv/bar/usr
#define TSL_FOR_EACH_SWITCH_ACTION(select)\
\
select( TSL_SwitchAction_NO_CHANGE = 0, @"",          @""                 )\
select( TSL_SwitchAction_off,           @"off",       @"Off"              )\
select( TSL_SwitchAction_read,          @"rd",        @"Read"             )\
select( TSL_SwitchAction_write,         @"wr",        @"Write"            )\
select( TSL_SwitchAction_inventory,     @"inv",       @"Inventory"        )\
select( TSL_SwitchAction_barcode,       @"bar",       @"Barcode"          )\
select( TSL_SwitchAction_user,          @"usr",       @"User Defined"     )


///
/// The types of switch action
///
#define USE_ENUM(e,p,s) e,

typedef enum { TSL_FOR_EACH_SWITCH_ACTION(USE_ENUM) } TSL_SwitchAction;

#undef USE_ENUM

///
/// A command to set the action of the reader's switch
///
/// The `TSL_SwitchAction` enum is used to specify values for the `singlePressAction` and `doublePressAction` parameters
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_SwitchAction_NO_CHANGE` - _The value has not been specified and the parameter will not be sent_
/// - `TSL_SwitchAction_off` - _'do nothing' has been specified and is sent as `'off'`_
/// - `TSL_SwitchAction_read` - _'.rd' command has been specified and is sent as `'rd'`_
/// - `TSL_SwitchAction_write` - _'.wr' command has been specified and is sent as `'wr'`_
/// - `TSL_SwitchAction_inventory` - _'.iv' command has been specified and is sent as `'inv'`_
/// - `TSL_SwitchAction_barcode` - _'.bc' command has been specified and is sent as `'bar'`_
/// - `TSL_SwitchAction_user` - _a 'user-defined' command has been specified and is sent as `'usr'`_
///
/// When a command includes the '-p' option the `singlePressAction` and `doublePressAction` parameters will be set to the reader's current values
///

@interface TSLSwitchActionCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

/// @name Properties

///
/// Set to `TSL_TriState_YES` to start the generation of asynchronous switch status reports
/// Set to `TSL_TriState_NO` to stop the generation of asynchronous switch status reports
/// Set to `TSL_TriState_NOT_SPECIFIED` to leave the asynchronous reporting state unchanged
///
/// if `readParameters` is specified then after execution this property will reflect the current state
///
@property (nonatomic, readwrite) TSL_TriState asynchronousReportingEnabled;

/// Set the Single press action
@property (nonatomic, readwrite) TSL_SwitchAction singlePressAction;

/// Set the Double press action
@property (nonatomic, readwrite) TSL_SwitchAction doublePressAction;

/// The minimum delay, in the range [1-999], before the single press trigger action repeats
/// Negative values are ignored and the delay is not set (the default value)
@property (nonatomic, readwrite) int singlePressRepeatDelay;

/// The minimum delay, in the range [1-999], before the double press trigger action repeats
/// Negative values are ignored and the delay is not set (the default value)
@property (nonatomic, readwrite) int doublePressRepeatDelay;



/// @name Factory Methods

///
/// Returns a synchronous TSLSwitchActionCommand
///
+(TSLSwitchActionCommand *)synchronousCommand;

///
///@return the minimum valid value for the repeat delay parameter
///
+(int)minimumRepeatDelay;

///
///@return the maximum valid value for the repeat delay parameter
///
+(int)maximumRepeatDelay;


/// Parse the given string and determine the corresponding TSL_SwitchAction
/// @param value
///
/// Parse the given parameter value string and determine the corresponding TSL_SwitchAction
///
///@param value The parameter value as NSString
///@return The TSL_SwitchAction corresponding to the given string. Invalid parameter values return TSL_SwitchAction_NO_CHANGE
///
+(TSL_SwitchAction)switchActionForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_SwitchAction
///
///@param action The TSL_SwitchAction value to be converted
///@return The command parameter value for the given TSL_SwitchAction or nil if invalid
///
+(NSString *)parameterValueForSwitchAction:(TSL_SwitchAction)action;

///
/// Get the description corresponding to the given TSL_SwitchAction
///
///@param action The TSL_SwitchAction value to be converted
///@return The user friendly name for the given TSL_SwitchAction or nil if invalid
///
+(NSString *)descriptionForSwitchAction:(TSL_SwitchAction)action;

///
/// Test the given TSL_SwitchAction for validity
///
///@param action The TSL_SwitchAction value to be tested
///@return YES if the given TSL_SwitchAction is valid otherwise NO
///
+(BOOL)isValidSwitchAction:(TSL_SwitchAction)action;

@end
