//
//  TSLCommandParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 08/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TSLParametersProtocol.h"
#import "TSLTriState.h"

///
/// Parameters for generic control of TSLAsciiCommands
///
@protocol TSLCommandParametersProtocol <NSObject>

/// @name Controlling the Command 

/// When set to `TSL_TriState_YES` prevents the reader from performing the command's primary action (e.g. TSLInventoryCommand will not perform the inventory action)
/// All other actions, such as setting parameters in the reader are performed
@property (nonatomic, readwrite) TSL_TriState takeNoAction;

/// When set to `TSL_TriState_YES` instructs the reader to return the supported parameters and their current values for the command
@property (nonatomic, readwrite) TSL_TriState readParameters;

/// When set to `TSL_TriState_YES` instructs the reader to reset all the parameter values for the command
@property (nonatomic, readwrite) TSL_TriState resetParameters;


/// @name Determining supported command parameters

/// Returns YES if the TSLAsciiCommand responds to the `takeNoAction` parameter
@property (nonatomic, readonly) BOOL implementsTakeNoAction;

/// Returns YES if the TSLAsciiCommand responds to the `readParameters` parameter
@property (nonatomic, readonly) BOOL implementsReadParameters;

/// Returns YES if the TSLAsciiCommand responds to the `resetParameters` parameter
@property (nonatomic, readonly) BOOL implementsResetParameters;

@end


///
/// Helper class for implementing TSLCommandParametersProtocol
///
@interface TSLCommandParameters : NSObject <TSLParametersProtocol>

@end
