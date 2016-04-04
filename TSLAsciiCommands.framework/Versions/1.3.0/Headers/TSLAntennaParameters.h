//
//  TSLAntennaParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 03/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

///
/// Parameters related to the reader Antenna operation during commands
///
@protocol TSLAntennaParametersProtocol <NSObject>

/// The antenna power to use for this command.
///
/// Valid power range is minimuOutputPower - maximumOutputPower.
/// Negative values are ignored and the power is not set.
/// Other values will throw a TSLArgumentOutOfRangeException when parameter is used to construct commandLine (see TSLParametersProtocol)
@property (nonatomic, readwrite) int outputPower;

/// The minimum valid output power value
+(int)minimumOutputPower;

/// The maximum valid output power value
+(int)maximumOutputPower;
@end


///
/// Helper class for implementing TSLAntennaParametersProtocol
///
@interface TSLAntennaParameters : NSObject <TSLParametersProtocol>

@end
