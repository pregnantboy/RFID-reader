//
//  TSLBarcodeCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 15/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLCommandParameters.h"
#import "TSLResponseParameters.h"

#import "TSLAsciiSelfResponderCommandBase.h"

///
/// Protocol for objects wishing to receive asynchronous notification of each barcode received
///
@protocol TSLBarcodeCommandBarcodeReceivedDelegate <NSObject>

///
/// The barcode information received
///
/// @param data the barcode information as ascii string
///
-(void)barcodeReceived:(NSString *)data;

@end


///
/// A command to read a barcode
///
@interface TSLBarcodeCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol, TSLResponseParametersProtocol>

/// @name Properties

/// Turn the escape character on/off
@property (nonatomic, readwrite) TSL_TriState useEscapeCharacter;

/// Specify the scan duration in seconds (1-9)
@property (nonatomic, readwrite) int scanTime;

/// A TSLBarcodeCommandBarcodeReceivedDelegate to inform when barcode data is received
@property (weak, nonatomic, readwrite) id<TSLBarcodeCommandBarcodeReceivedDelegate>barcodeReceivedDelegate;

/// The data received from the barcode scan
@property (nonatomic, readonly) NSString *data;


/// @name Factory Methods

///
/// Returns a synchronous TSLBarcodeCommand
///
+(TSLBarcodeCommand *)synchronousCommand;

///
/// Returns a synchronous TSLBarcodeCommand for the given scan timeout
///
/// @param timeout the timeout in seconds (1-9)
///
+(TSLBarcodeCommand *)synchronousCommandWithTimeout:(int)timeout;

@end
