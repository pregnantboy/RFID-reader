//
//  TSLDeleteConfirmation.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 07/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum
{
    TSL_DeleteConfirmation_NO = 0,
    TSL_DeleteConfirmation_YES,
}
TSL_DeleteConfirmation;

///
/// Helper class for TSL_DeleteConfirmation enum
///
/// The TSL_DeleteConfirmation enum is used to specify values for TSLAsciiCommand parameters.
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_DeleteConfirmation_NO` - _The value has not been specified and the corresponding parameter will not be sent_
/// - `TSL_DeleteConfirmation_YES` - _The value has been specified and is sent as `'on'`_
///
@interface TSLDeleteConfirmation : NSObject

///
/// Get the parameter value corresponding to the given TSL_DeleteConfirmation
///
///@param state The TSL_DeleteConfirmation value to be converted
///@return The command parameter value for the given TSL_DeleteConfirmation or nil if invalid
///
+(NSString *)parameterValueForDeleteConfirmation:(TSL_DeleteConfirmation)state;

///
/// Get the description corresponding to the given TSL_DeleteConfirmation
///
///@param state The TSL_DeleteConfirmation value to be converted
///@return The user friendly name for the given TSL_DeleteConfirmation or nil if invalid
///
+(NSString *)descriptionForDeleteConfirmation:(TSL_DeleteConfirmation)state;

///
/// Parse the given parameter value string and determine the corresponding TSL_DeleteConfirmation
///
///@param value The parameter value as NSString
///@return The TSL_DeleteConfirmation corresponding to the given string. Invalid parameter values return TSL_DeleteConfirmation_NO
///
+(TSL_DeleteConfirmation)deleteConfirmationForParameterValue:(NSString *)value;

@end
