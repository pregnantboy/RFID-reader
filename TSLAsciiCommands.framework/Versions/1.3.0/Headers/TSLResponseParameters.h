//
//  TSLResponseParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 03/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLParametersProtocol.h"
#import "TSLTriState.h"


///
/// Generic parameters for command responses
///
@protocol TSLResponseParametersProtocol <NSObject>

/// Control the use of alerts from the reader when executing commands
@property (nonatomic, readwrite) TSL_TriState useAlert;

/// Control the presence of DateTime stamps in reader responses
@property (nonatomic, readwrite) TSL_TriState includeDateTime;

@end

///
/// Helper class for implementing TSLResponseParametersProtocol
///
@interface TSLResponseParameters : NSObject <TSLParametersProtocol>

///
/// Constructs the partial command line required for a TSL ASCII 2.0 Reader based on the properties of the given source object
///
/// @param source The object whose properties are to be encoded
/// @param includeAlert YES if the alert parameter is to be included in the command line
/// @return The command line fragment built from the source objects properties
///
+(NSString *)commandLine:(id<TSLResponseParametersProtocol>)source includeAlert:(BOOL)includeAlert;

@end
