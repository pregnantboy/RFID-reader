//
//  TSLWriteTransponderCommand.h
//  AgoutiCommands
//
//  Created by Brian Painter on 10/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLTransponderMemoryCommandBase.h"

///
/// Protocol for objects wishing to receive asynchronous notification of each transponder received
///
@protocol TSLWriteSingleCommandTransponderReceivedDelegate <NSObject>

///
/// The transponder information received
///
/// @param epc the transponder identifier as hex-encoded, ascii string
/// @param crc the transponder CRC as unsigned short encoded in NSNumber or nil if none
/// @param pc the transponder program control word as unsigned short encoded in NSNumber or nil if none
/// @param rssi the transponders RSSI value (in dBm) as int encoded in NSNumber or nil if none
/// @param index the index value as unsigned short encoded in NSNumber or nil if none
/// @param wordsWritten the number of words successfully written to the transponder as int encoded in NSNumber or nil if none
/// @param moreAvailable YES if there are more transponders to be delivered
///
-(void)transponderReceivedFromWrite:(NSString *)epc crc:(NSNumber *)crc pc:(NSNumber *)pc rssi:(NSNumber *)rssi index:(NSNumber *)index wordsWritten:(NSNumber *)wordsWritten moreAvailable:(BOOL)moreAvailable;

@end


///
/// The block type to handle transponder information received
///
/// @param epc the transponder identifier as hex-encoded, ascii string
/// @param crc the transponder CRC as unsigned short encoded in NSNumber or nil if none
/// @param pc the transponder program control word as unsigned short encoded in NSNumber or nil if none
/// @param rssi the transponders RSSI value (in dBm) as int encoded in NSNumber or nil if none
/// @param index the index value as unsigned short encoded in NSNumber or nil if none
/// @param wordsWritten the number of words successfully written to the transponder as int encoded in NSNumber or nil if none
/// @param moreAvailable YES if there are more transponders to be delivered
typedef void(^TSLWriteSingleTransponderReceivedBlock)(NSString *epc, NSNumber *crc, NSNumber *pc, NSNumber *rssi, NSNumber *index, NSNumber *wordsWritten, BOOL moreAvailable);



///
/// A command to write data to the memory banks of a single transponder only
///
@interface TSLWriteSingleTransponderCommand : TSLTransponderMemoryCommandBase

/// @name Properties

/// The TSLWriteSingleCommandTransponderReceivedDelegate to inform when each transponder is received
@property (weak, nonatomic, readwrite) id<TSLWriteSingleCommandTransponderReceivedDelegate>transponderReceivedDelegate;

/// The Block invoked for each transponder received
@property (nonatomic, copy) TSLWriteSingleTransponderReceivedBlock transponderReceivedBlock;

/// YES if a transponder responded to the write command. 
@property (nonatomic, readonly) BOOL didAnyTransponderRespond;


/// @name Factory Methods

///
/// Returns a synchronous TSLWriteSingleTransponderCommand
/// (with self as synchronous responder)
///
+(TSLWriteSingleTransponderCommand *)synchronousCommand;

@end
