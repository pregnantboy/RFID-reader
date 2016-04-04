//
//  TSLQueryParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 04/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLParametersProtocol.h"


///
/// Use macro to allow the creation of enum, parameter value and descriptive strings that are always in sync
///
#define USE_ENUM(e,p,s) e,

/// Query Select values
#define TSL_FOR_EACH_QUERY_SELECT(select)\
\
select( TSL_QuerySelect_NotSpecified = 0,  @"",       @"Not specified"  )\
select( TSL_QuerySelect_All,               @"all",    @"All"           )\
select( TSL_QuerySelect_NSL,               @"nsl",    @"Not Selected"  )\
select( TSL_QuerySelect_SL,                @"sl",     @"Selected"       )\


///
/// The query select type
///
typedef enum { TSL_FOR_EACH_QUERY_SELECT(USE_ENUM) } TSL_QuerySelect;


/// Query Session Values
#define TSL_FOR_EACH_QUERY_SESSION(select)\
\
select( TSL_QuerySession_NotSpecified = 0,  @"",       @"Not specified"     )\
select( TSL_QuerySession_S0,                @"s0",     @"Session 0"         )\
select( TSL_QuerySession_S1,                @"s1",     @"Session 1"         )\
select( TSL_QuerySession_S2,                @"s2",     @"Session 2"         )\
select( TSL_QuerySession_S3,                @"s3",     @"Session 3"         )

///
/// The query session type
///
typedef enum { TSL_FOR_EACH_QUERY_SESSION(USE_ENUM) } TSL_QuerySession;


/// Query Session Values
#define TSL_FOR_EACH_QUERY_TARGET(select)\
\
select( TSL_QueryTarget_NotSpecified = 0,  @"",      @"Not specified"     )\
select( TSL_QueryTarget_A,                 @"a",     @"A"         )\
select( TSL_QueryTarget_B,                 @"b",     @"B"         )

///
/// The query session type
///
typedef enum { TSL_FOR_EACH_QUERY_TARGET(USE_ENUM) } TSL_QueryTarget;


#undef USE_ENUM


///
/// Parameters for Query operations in commands and responses
///
@protocol TSLQueryParametersProtocol <NSObject>

/// The transponders to include based on the select flag state
@property (nonatomic, readwrite) TSL_QuerySelect querySelect;

/// The session of the transponders to be included in this operation
@property (nonatomic, readwrite) TSL_QuerySession querySession;

/// The session state of the transponders to be included in this operation
@property (nonatomic, readwrite) TSL_QueryTarget queryTarget;

@end


///
/// Helper class for implementing TSLQueryParametersProtocol
///
/// The `TSL_QuerySelect` enum is used to specify values for the querySelect parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_QuerySelect_NOT_SPECIFIED` - _The value has not been specified and the `querySelect` parameter will not be sent_
/// - `TSL_QuerySelect_All` - _All transponders has been specified and is sent as `'all'`_
/// - `TSL_QuerySelect_NSL` - _Not-selected transponders has been specified and is sent as `'nsl'`_
/// - `TSL_QuerySelect_SL` - _Selected transponders has been specified and is sent as `'sl'`_
///
/// When a command includes the '-p' option the `querySelect` parameter will be set to the reader's current value
///
/// The `TSL_QuerySession` enum is used to specify values for the querySession parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_QuerySession_NOT_SPECIFIED` - _The value has not been specified and the `querySession` parameter will not be sent_
/// - `TSL_QuerySession_S0` - _Session 0 has been specified and is sent as `'s0'`_
/// - `TSL_QuerySession_S1` - _Session 1 has been specified and is sent as `'s1'`_
/// - `TSL_QuerySession_S2` - _Session 2 has been specified and is sent as `'s2'`_
/// - `TSL_QuerySession_S3` - _Session 3 has been specified and is sent as `'s3'`_
///
/// When a command includes the '-p' option the `querySession` parameter will be set to the reader's current value
///
/// The `TSL_QueryTarget` enum is used to specify values for the queryTarget parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_QueryTarget_NOT_SPECIFIED` - _The value has not been specified and the `queryTarget` parameter will not be sent_
/// - `TSL_QueryTarget_A` - _The A state has been specified and is sent as `'a'`_
/// - `TSL_QueryTarget_B` - _The B state has been specified and is sent as `'b'`_
///
/// When a command includes the '-p' option the `queryTarget` parameter will be set to the reader's current value
///

@interface TSLQueryParameters : NSObject <TSLParametersProtocol>

/// @name Query Select methods

///
/// Parse the given parameter value string and determine the corresponding TSL_QuerySelect
///
///@param value The parameter value as NSString
///@return The TSL_QuerySelect corresponding to the given string. Invalid parameter values return TSL_QuerySelect_NotSpecified
///
+(TSL_QuerySelect)querySelectForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_QuerySelect
///
///@param select The TSL_QuerySelect value to be converted
///@return The command parameter value for the given TSL_QuerySelect or nil if invalid
///
+(NSString *)parameterValueForQuerySelect:(TSL_QuerySelect)select;


///
/// Get the description corresponding to the given TSL_QuerySelect
///
///@param select The TSL_QuerySelect value to be converted
///@return The user friendly name for the given TSL_QuerySelect or nil if invalid
///

+(NSString *)descriptionForQuerySelect:(TSL_QuerySelect)select;

///
/// Test the given TSL_QuerySelect for validity
///
///@param select The TSL_QuerySelect value to be tested
///@return YES if the given TSL_QuerySelect is valid otherwise NO
///
+(BOOL)isValidQuerySelect:(TSL_QuerySelect)select;


/// @name Query Session methods

///
/// Parse the given parameter value string and determine the corresponding TSL_QuerySession
///
///@param value The parameter value as NSString
///@return The TSL_QuerySession corresponding to the given string. Invalid parameter values return TSL_QuerySession_NotSpecified
///
+(TSL_QuerySession)querySessionForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_QuerySession
///
///@param session The TSL_QuerySession value to be converted
///@return The command parameter value for the given TSL_QuerySession or nil if invalid
///
+(NSString *)parameterValueForQuerySession:(TSL_QuerySession)session;


///
/// Get the description corresponding to the given TSL_QuerySession
///
///@param session The TSL_QuerySession value to be converted
///@return The user friendly name for the given TSL_QuerySession or nil if invalid
///
+(NSString *)descriptionForQuerySession:(TSL_QuerySession)session;

///
/// Test the given TSL_QuerySession for validity
///
///@param session The TSL_QuerySession value to be tested
///@return YES if the given TSL_QuerySession is valid otherwise NO
///
+(BOOL)isValidQuerySession:(TSL_QuerySession)session;


/// @name Query Target methods

///
/// Parse the given parameter value string and determine the corresponding TSL_QueryTarget
///
///@param value The parameter value as NSString
///@return The TSL_QueryTarget corresponding to the given string. Invalid parameter values return TSL_QueryTarget_NotSpecified
///
+(TSL_QueryTarget)queryTargetForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_QueryTarget
///
///@param target The TSL_QueryTarget value to be converted
///@return The command parameter value for the given TSL_QueryTarget or nil if invalid
///
+(NSString *)parameterValueForQueryTarget:(TSL_QueryTarget)target;


///
/// Get the description corresponding to the given TSL_QueryTarget
///
///@param target The TSL_QueryTarget value to be converted
///@return The user friendly name for the given TSL_QueryTarget or nil if invalid
///
+(NSString *)descriptionForQueryTarget:(TSL_QueryTarget)target;

///
/// Test the given TSL_QueryTarget for validity
///
///@param target The TSL_QueryTarget value to be tested
///@return YES if the given TSL_QueryTarget is valid otherwise NO
///
+(BOOL)isValidQueryTarget:(TSL_QueryTarget)target;



@end
