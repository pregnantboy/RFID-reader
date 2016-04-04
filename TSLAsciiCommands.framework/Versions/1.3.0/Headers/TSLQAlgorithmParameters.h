//
//  TSLQAlgorithmParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 05/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLParametersProtocol.h"

///
/// Use macro to allow the creation of enum, parameter value and descriptive strings that are always in sync
///
#define USE_ENUM(e,p,s) e,

/// Q Algorithm values
#define TSL_FOR_EACH_Q_ALGORITHM(select)\
\
select( TSL_QAlgorithm_NotSpecified = 0,  @"",       @"Not specified"   )\
select( TSL_QAlgorithm_Fixed,             @"fix",    @"Fixed"           )\
select( TSL_QAlgorithm_Dynamic,           @"dyn",    @"Dynamic"         )

///
/// The Q Algorithm type
///
typedef enum { TSL_FOR_EACH_Q_ALGORITHM(USE_ENUM) } TSL_QAlgorithm;

#undef USE_ENUM



///
/// Parameters for Q Algorithm in commands and responses
///
@protocol TSLQAlgorithmParametersProtocol <NSObject>

/// Q algorithm type
@property (nonatomic, readwrite) TSL_QAlgorithm qAlgorithm;

/// Q value for fixed Q operations (0-15)
@property (nonatomic, readwrite) int qValue;

@end

///
/// Helper class for implementing TSLQAlgorithmParametersProtocol
///
/// The `TSL_QAlgorithm` enum is used to specify values for the qAlgorithm parameter.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_QAlgorithm_NOT_SPECIFIED` - _The value has not been specified and the `qAlgorithm` parameter will not be sent_
/// - `TSL_QAlgorithm_Fixed` - _Fixed time-slots has been specified and is sent as `'fix'`_
/// - `TSL_QAlgorithm_Dynamic` - _Dynamic time-slots has been specified and is sent as `'dyn'`_
///
/// When a command includes the '-p' option the `qAlgorithm` parameter will be set to the reader's current value
///
@interface TSLQAlgorithmParameters : NSObject <TSLParametersProtocol>

///
/// Constructs the partial command line required for a TSL ASCII 2.0 Reader based on the properties of the given source object
///
/// @param source The object whose properties are to be encoded
/// @param includeAlgorithm YES if the Q algorithm parameter is to be included in the command line
/// @return The command line fragment built from the source objects properties
///
+(NSString *)commandLine:(id<TSLQAlgorithmParametersProtocol>)source includeAlgorithm:(BOOL)includeAlgorithm;


/// @name Q Algorithm methods

///
/// Parse the given parameter value string and determine the corresponding TSL_QAlgorithm
///
///@param value The parameter value as NSString
///@return The TSL_QAlgorithm corresponding to the given string. Invalid parameter values return TSL_QAlgorithm _NotSpecified
///
+(TSL_QAlgorithm)qAlgorithmForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_QAlgorithm
///
///@param algorithm The TSL_QAlgorithm value to be converted
///@return The command parameter value for the given TSL_QAlgorithm or nil if invalid
///
+(NSString *)parameterValueForQAlgorithm:(TSL_QAlgorithm)algorithm;


///
/// Get the description corresponding to the given TSL_QAlgorithm
///
///@param algorithm The TSL_QAlgorithm value to be converted
///@return The user friendly name for the given TSL_QAlgorithm or nil if invalid
///
+(NSString *)descriptionForQAlgorithm:(TSL_QAlgorithm)algorithm;

///
/// Test the given TSL_QAlgorithm for validity
///
///@param algorithm The TSL_QAlgorithm value to be tested
///@return YES if the given TSL_QAlgorithm is valid otherwise NO
///
+(BOOL)isValidQAlgorithm:(TSL_QAlgorithm)algorithm;

@end
