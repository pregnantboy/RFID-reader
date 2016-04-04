//
//  TSLTransponderMemoryCommandBase.h
//  AgoutiCommands
//
//  Created by Brian Painter on 09/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAllParameters.h"

#import "TSLAsciiSelfResponderCommandBase.h"


///
/// A base class for read/write commands
///
/// This provides common behaviour, properties and parameters for the read and write commands. Many properties are defined in the parameter protocols this class implements
///
@interface TSLTransponderMemoryCommandBase : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol, TSLDataBankParametersProtocol, TSLAntennaParametersProtocol, TSLResponseParametersProtocol, TSLSelectControlParametersProtocol, TSLSelectMaskParametersProtocol, TSLTransponderParametersProtocol>

/// @name Properties

/// The EPC identifier returned by the reader
@property (nonatomic, readonly) NSString *epcResponse;

/// The CRC value returned by the reader
@property (nonatomic, readonly) NSNumber *crcResponse;

/// The PC value returned by the reader
@property (nonatomic, readonly) NSNumber *pcResponse;

/// The RSSI value returned by the reader
@property (nonatomic, readonly) NSNumber *rssiResponse;

/// The Index value returned by the reader
@property (nonatomic, readonly) NSNumber *indexResponse;

///
/// Clears the data from the last transponder seen
///
/// Override this method to clear additional data from derived classes
/// Ensure that the super class implementation is also called
///
-(void)clearLastTransponder;

@end
