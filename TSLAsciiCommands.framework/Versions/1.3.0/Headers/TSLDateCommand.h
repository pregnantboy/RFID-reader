//
//  TSLDateCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 19/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to obtain or set the date of the reader's real-time clock
///
@interface TSLDateCommand : TSLAsciiSelfResponderCommandBase

/// @name Properties

///
/// Set date to nil to read the device's date
/// Set date to a valid NSDate to set the device's date
///
/// Default value is nil
///
/// *Warning:* Only the Date section of the NSDate object is valid
///
@property (nonatomic, readwrite) NSDate *date;

/// @name Factory Methods

///
/// Returns a synchronous TSLDateCommand
///
+(TSLDateCommand *)synchronousCommand;

///
/// Returns a synchronous TSLDateCommand set to the given NSDate
///
/// @param date The value for the command's `date` property
///
+(TSLDateCommand *)synchronousCommandWithDate:(NSDate *)date;


@end
