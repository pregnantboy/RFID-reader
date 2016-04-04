//
//  TSLLicenceKeyCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 19/08/2014.
//  Copyright (c) 2014 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>


///
/// A command to read, write and delete the licence key stored in the reader's non-volatile memory.
///
///
@interface TSLLicenceKeyCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol>

/// The maximum allowed licence key length
+(int)maximumLicenceKeyLength;

/// Set the licence key for the device
@property (nonatomic, readwrite) NSString *licenceKey;


/// Set to TSL_DeleteConfirmation_YES to delete the existing licence key from the device
@property (nonatomic, readwrite) TSL_DeleteConfirmation deleteLicenceKey;

///
/// Returns a synchronous TSLLicenceKeyCommand
///
+(TSLLicenceKeyCommand *)synchronousCommand;

@end
