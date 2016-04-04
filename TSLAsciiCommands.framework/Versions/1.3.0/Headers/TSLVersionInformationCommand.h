//
//  TSLVersionInformationCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 18/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TSLAsciiSelfResponderCommandBase.h"

///
/// A command to query the reader for version information
///
@interface TSLVersionInformationCommand : TSLAsciiSelfResponderCommandBase

/// @name Properties

/// The Manufacturer name retrieved from the reader
@property (nonatomic, readonly) NSString *manufacturer;

/// The Unit Serial number retrieved from the reader
@property (nonatomic, readonly) NSString *serialNumber;

/// The Unit Firmware version number retrieved from the reader
@property (nonatomic, readonly) NSString *firmwareVersion;

/// The Unit bootloader version number retrieved from the reader
@property (nonatomic, readonly) NSString *bootloaderVersion;

/// The Radio Serial number retrieved from the reader
@property (nonatomic, readonly) NSString *radioSerialNumber;

/// The Radio Firmware version number retrieved from the reader
@property (nonatomic, readonly) NSString *radioFirmwareVersion;

/// The Radio Bootloader version number retrieved from the reader
@property (nonatomic, readonly) NSString *radioBootloaderVersion;

/// The Antenna Serial number retrieved from the reader
@property (nonatomic, readonly) NSString *antennaSerialNumber;

/// The Protocol version number retrieved from the reader
@property (nonatomic, readonly) NSString *asciiProtocol;

/// The Bluetooth address retrieved from the reader
@property (nonatomic, readonly) NSString *bluetoothAddress;



/// @name Factory Methods

///
/// Returns a synchronous TSLVersionInformationCommand with a default TSLVersionInformationCommandResponder
///
+(TSLVersionInformationCommand *)synchronousCommand;



@end
