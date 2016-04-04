//
//  TSLSelectParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 04/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLTriState.h"
#import "TSLParametersProtocol.h"

///
/// Use macro to allow the creation of enum, parameter value and descriptive strings that are always in sync
///
#define USE_ENUM(e,p,s) e,

/// Select Target
#define TSL_FOR_EACH_SELECT_TARGET(select)\
\
select( TSL_SelectTarget_NotSpecified = 0,  @"",       @"Not specified"     )\
select( TSL_SelectTarget_S0,                @"s0",     @"Session 0"         )\
select( TSL_SelectTarget_S1,                @"s1",     @"Session 1"         )\
select( TSL_SelectTarget_S2,                @"s2",     @"Session 2"         )\
select( TSL_SelectTarget_S3,                @"s3",     @"Session 3"         )\
select( TSL_SelectTarget_SL,                @"sl",     @"Select"            )


///
/// The select target types
///
typedef enum { TSL_FOR_EACH_SELECT_TARGET(USE_ENUM) } TSL_SelectTarget;

#undef USE_ENUM

/// Select Action
#define TSL_FOR_EACH_SELECT_ACTION(select)\
\
select( TSL_SelectAction_NotSpecified = -1,             @"Not specified"                                                                     )\
select( TSL_SelectAction_AssertSetA_DeassertSetB = 0,   @"Match: Assert Select / Set Session A  Non Match: Deassert Select / Set Session B"  )\
select( TSL_SelectAction_AssertSetA_NothingNothing,     @"Match: Assert Select / Set Session A  Non Match: Nothing / Nothing"                )\
select( TSL_SelectAction_NothingNothing_DeassertSetB,   @"Match: Nothing / Nothing  Non Match: Deassert Select / Set Session B"              )\
select( TSL_SelectAction_ToggleToggle_NothingNothing,   @"Match: Toggle / Toggle  Non Match: Nothing / Nothing"                              )\
select( TSL_SelectAction_DeassertSetB_AssertSetA,       @"Match: Deassert Select / Set Session B  Non Match: Assert Select / Set Session A"  )\
select( TSL_SelectAction_DeassertSetB_NothingNothing,   @"Match: Deassert Select / Set Session B  Non Match: Nothing / Nothing"              )\
select( TSL_SelectAction_NothingNothing_AssertSetA,     @"Match: Nothing / Nothing  Non Match: Assert Select / Set Session A"                )\
select( TSL_SelectAction_NothingNothing_ToggleToggle,   @"Match: Nothing / Nothing  Non Match: Toggle / Toggle"                              )


///
/// The select target types
///
#define USE_ENUM(e,s) e,

typedef enum { TSL_FOR_EACH_SELECT_ACTION(USE_ENUM) } TSL_SelectAction;

#undef USE_ENUM


///
/// Parameters for Select Control operations in commands and responses
///
@protocol TSLSelectControlParametersProtocol <NSObject>

/// @name Controlling the Select operation

/// The target flag for the Select operation
@property (nonatomic, readwrite) TSL_SelectTarget selectTarget;

/// The action to perform in the Select operation
@property (nonatomic, readwrite) TSL_SelectAction selectAction;

@end

///
/// Helper class for implementing TSLSelectControlParametersProtocol
///
/// The `TSL_SelectTarget` enum is used to specify values for the `selectTarget` parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_SelectTarget_NOT_SPECIFIED` - _The value has not been specified and the `selectTarget` parameter will not be sent_
/// - `TSL_SelectTarget_S0` - _Session 0 has been specified and is sent as `'s0'`_
/// - `TSL_SelectTarget_S1` - _Session 1 has been specified and is sent as `'s1'`_
/// - `TSL_SelectTarget_S2` - _Session 2 has been specified and is sent as `'s2'`_
/// - `TSL_SelectTarget_S3` - _Session 3 has been specified and is sent as `'s3'`_
/// - `TSL_SelectTarget_SL` - _Select has been specified and is sent as `'sl'`_
///
/// When a command includes the '-p' option the `selectTarget` parameter will be set according to the reader's current value
///
///
/// The `TSL_SelectAction` enum is used to specify values for the selectAction parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_SelectAction_NOT_SPECIFIED` - _The value has not been specified and the `selectAction` parameter will not be sent_
/// - `TSL_SelectAction_AssertSetA_DeassertSetB` - _Match: Assert Select / Set Session A  Non Match: Deassert Select / Set Session B_
/// - `TSL_SelectAction_AssertSetA_NothingNothing` - _Match: Assert Select / Set Session A  Non Match: Nothing / Nothing_
/// - `TSL_SelectAction_NothingNothing_DeassertSetB` - _Match: Nothing / Nothing  Non Match: Deassert Select / Set Session B_
/// - `TSL_SelectAction_ToggleToggle_NothingNothing` - _Match: Toggle / Toggle  Non Match: Nothing / Nothing_
/// - `TSL_SelectAction_DeassertSetB_AssertSetA` - _Match: Deassert Select / Set Session B  Non Match: Assert Select / Set Session A_
/// - `TSL_SelectAction_DeassertSetB_NothingNothing` - _Match: Deassert Select / Set Session B  Non Match: Nothing / Nothing_
/// - `TSL_SelectAction_NothingNothing_AssertSetA` - _Match: Nothing / Nothing  Non Match: Assert Select / Set Session A_
/// - `TSL_SelectAction_NothingNothing_ToggleToggle` - _Match: Nothing / Nothing  Non Match: Toggle / Toggle_
///
/// When a command includes the '-p' option the `selectAction` parameter will be set according to the reader's current value
///
@interface TSLSelectControlParameters : NSObject <TSLParametersProtocol>

///@name Select Target helper methods

///
/// Parse the given parameter value string and determine the corresponding TSL_SelectTarget
///
///@param value The parameter value as NSString
///@return The TSL_SelectTarget corresponding to the given string. Invalid parameter values return TSL_SelectTarget_NotSpecified
///
+(TSL_SelectTarget)selectTargetForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_SelectTarget
///
///@param target The TSL_SelectTarget value to be converted
///@return The command parameter value for the given TSL_SelectTarget or nil if invalid
///

+(NSString *)parameterValueForSelectTarget:(TSL_SelectTarget)target;

///
/// Get the description corresponding to the given TSL_SelectTarget
///
///@param target The TSL_SelectTarget value to be converted
///@return The user friendly name for the given TSL_SelectTarget or nil if invalid
///
+(NSString *)descriptionForSelectTarget:(TSL_SelectTarget)target;

///
/// Test the given TSL_SelectTarget for validity
///
///@param target The TSL_SelectTarget value to be tested
///@return YES if the given TSL_SelectTarget is valid otherwise NO
///
+(BOOL)isValidSelectTarget:(TSL_SelectTarget)target;



///@name Select Action helper methods

///
/// Get the description corresponding to the given TSL_SelectAction
///
///@param action The TSL_SelectAction value to be converted
///@return The user friendly name for the given TSL_SelectAction or nil if invalid
///
+(NSString *)descriptionForSelectAction:(TSL_SelectAction)action;

///
/// Test the given TSL_SelectAction for validity
///
///@param action The TSL_SelectAction value to be tested
///@return YES if the given TSL_SelectAction is valid otherwise NO
///
+(BOOL)isValidSelectAction:(TSL_SelectAction)action;


@end
