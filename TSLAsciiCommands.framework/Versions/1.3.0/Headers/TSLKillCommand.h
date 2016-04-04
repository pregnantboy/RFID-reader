//
//  TSLKillCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 10/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// The block type to handle transponder information received
///
/// @param epc - the transponder identifier as hex-encoded, ascii string
/// @param crc - the transponder CRC as unsigned short encoded in NSNumber or nil if none
/// @param pc - the transponder program control word as unsigned short encoded in NSNumber or nil if none
/// @param rssi - the transponders RSSI value (in dBm) as int encoded in NSNumber or nil if none
/// @param index - the index value as unsigned short encoded in NSNumber or nil if none
/// @param killSucceeded - YES if the operation succeeded
/// @param moreAvailable - YES if there are more transponders to be delivered
///
typedef void(^TSLKillTransponderReceivedBlock)(NSString *epc, NSNumber *crc, NSNumber *pc, NSNumber *rssi, NSNumber *index, BOOL killSucceeded, BOOL moreAvailable);


///
/// A command to kill transponders
///
/// Configure this command using the properties defined in the parameter protocols this class implements
///
@interface TSLKillCommand : TSLAsciiSelfResponderCommandBase
    <
        TSLCommandParametersProtocol,
        TSLAntennaParametersProtocol,
        TSLTransponderParametersProtocol,
        TSLResponseParametersProtocol,
        TSLQueryParametersProtocol,
        TSLQAlgorithmParametersProtocol,
        TSLSelectMaskParametersProtocol,
        TSLSelectControlParametersProtocol
    >


/// The password required to kill transponders - defaults to nil
@property (nonatomic, readwrite) NSString *killPassword;

/// When set to `TSL_TriState_YES` the Select operation is NOT performed
@property (nonatomic, readwrite) TSL_TriState inventoryOnly;

/// YES if a transponder responded to the write command.
@property (nonatomic, readonly) BOOL didAnyTransponderRespond;

/// The Block invoked for each transponder response
@property (nonatomic, copy) TSLKillTransponderReceivedBlock transponderReceivedBlock;


/// @name Factory Methods

///
/// Returns a synchronous TSLKillCommand
///
+(TSLKillCommand *)synchronousCommand;




@end
