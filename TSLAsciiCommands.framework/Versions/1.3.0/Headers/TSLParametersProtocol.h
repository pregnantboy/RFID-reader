//
//  TSLParametersProtocol.h
//  AgoutiCommands
//
//  Created by Brian Painter on 03/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

///
/// Protocol for implementing TSLAsciiCommand properties
///
@protocol TSLParametersProtocol <NSObject>

///
/// Constructs the partial command line required for a TSL ASCII 2.0 Reader based on the properties of the given source object
///
/// @param source The object whose properties are to be encoded
/// @return The command line fragment built from the source objects properties
///
+(NSString *)commandLine:(id)source;

///
/// Examines the parameter string and, if recognised, sets the corresponding property on the object
///
///
/// @param parameter The TSL ASCII 2.0 parameter (without leading '-' e.g. 'b on')
/// @param object The object whose parameters are to be set
/// @return YES if property was recognised
///
+(BOOL)parseParameter:(NSString *)parameter forObject:(id)object;

///
/// Sets the given objects parameters to default values
/// @param object The object whose parameters are to be set
///
+(void)setDefaultParametersForObject:(id)object;

@end
