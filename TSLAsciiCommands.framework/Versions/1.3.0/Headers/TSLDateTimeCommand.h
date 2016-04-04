//
//  TSLDateTimeCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 20/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to obtain or set both date and time of the reader's real-time clock
///
@interface TSLDateTimeCommand : TSLAsciiSelfResponderCommandBase

/// @name Properties

///
/// Set date to nil to read the device's date and time
/// Set date to a valid NSDate to set the device's date and time
///
/// Default value is nil
///
@property (nonatomic, readwrite) NSDate *date;

/// @name Factory Methods

///
/// Returns a synchronous TSLDateTimeCommand
///
+(TSLDateTimeCommand *)synchronousCommand;

///
/// Returns a synchronous TSLDateTimeCommand set to the given NSDate
///
/// @param date The value for the command's `date` property
///
+(TSLDateTimeCommand *)synchronousCommandWithDate:(NSDate *)date;

@end
