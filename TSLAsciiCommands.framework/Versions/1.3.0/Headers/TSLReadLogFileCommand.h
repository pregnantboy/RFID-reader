//
//  TSLReadLogFileCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 08/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// A command to read the device's Log file
///
/// File commands require that they are the first responder in the chain to consume lines
/// So, to run this command synchronously requires that the Synchronous responder is the first (consuming) responder in the chain
/// (e.g. TSLLoggerResponder can come before it as it does NOT consume the lines it sees)
///
/// @warning The lines received from the device are not accumulated in the response property
/// and must be handled via the fileLineReceivedBlock
///
@interface TSLReadLogFileCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

/// Configure the command logging
@property (nonatomic, readwrite) TSL_TriState commandLoggingEnabled;

/// Set to TSL_DeleteConfirmation_YES to delete the log file from the device
@property (nonatomic, readwrite) TSL_DeleteConfirmation deleteFile;

/// The block executed for each file line received
@property (nonatomic, copy) TSLFileLineReceivedBlock fileLineReceivedBlock;


///
/// Returns a synchronous TSLReadLogFileCommand
///
+(TSLReadLogFileCommand *)synchronousCommand;


@end
