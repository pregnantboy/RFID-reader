//
//  TSLBatteryStatusCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 19/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to query the reader for battery status information
///
/// Note: This does not yet expose the charging status via a property but the 'CH:' line is captured in the response property
///
@interface TSLBatteryStatusCommand : TSLAsciiSelfResponderCommandBase

/// @name Properties

/// The battery level retrieved from the reader
@property (nonatomic, readonly) int batteryLevel;


/// @name Factory Methods

///
/// Returns a synchronous TSLBatteryStatusCommand
///
+(TSLBatteryStatusCommand *)synchronousCommand;


@end
