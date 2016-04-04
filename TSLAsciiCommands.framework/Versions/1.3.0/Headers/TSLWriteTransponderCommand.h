//
//  TSLWriteTransponderCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 09/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

#import "TSLTransponderMemoryCommandBase.h"

///
/// Write mode
///
/// Use macro to allow the creation of enum, command parameter and descriptive strings that are always in sync
///
#define TSL_FOR_EACH_WRITE_MODE(select)\
\
select( TSL_WriteMode_NotSpecified = 0,  @"",          @""                 )\
select( TSL_WriteMode_single,            @"s",         @"Single"           )\
select( TSL_WriteMode_block,             @"b",         @"Block"            )


///
/// The types of Write Modes
///
#define USE_ENUM(e,p,s) e,

typedef enum { TSL_FOR_EACH_WRITE_MODE(USE_ENUM) } TSL_WriteMode;

#undef USE_ENUM



///
/// Impinj Extensions: Block write mode
///
/// Use macro to allow the creation of enum, command parameter and descriptive strings that are always in sync
///
#define TSL_FOR_EACH_IMPINJ_BLOCK_WRITE_MODE(select)\
\
select( TSL_ImpinjBlockWriteMode_NotSpecified  = 0, @"",          @""               )\
select( TSL_ImpinjBlockWriteMode_auto,              @"a",         @"Auto"           )\
select( TSL_ImpinjBlockWriteMode_force1,            @"1",         @"Force 1"        )\
select( TSL_ImpinjBlockWriteMode_force2,            @"2",         @"Force 2"        )


///
/// The types of Impinj Write Modes
///
#define USE_ENUM(e,p,s) e,

typedef enum { TSL_FOR_EACH_IMPINJ_BLOCK_WRITE_MODE(USE_ENUM) } TSL_ImpinjBlockWriteMode;

#undef USE_ENUM





///
/// Protocol for objects wishing to receive asynchronous notification of each transponder received
///
@protocol TSLWriteCommandTransponderReceivedDelegate <NSObject>

///
/// The transponder information received
///
/// @param epc the transponder identifier as hex-encoded, ascii string
/// @param crc the transponder CRC as unsigned short encoded in NSNumber or nil if none
/// @param pc the transponder program control word as unsigned short encoded in NSNumber or nil if none
/// @param rssi the transponders RSSI value (in dBm) as int encoded in NSNumber or nil if none
/// @param index the index value as unsigned short encoded in NSNumber or nil if none
/// @param aErrorCode the access error code as NSString or nil if none
/// @param bErrorCode the backscatter error code as NSString or nil if none
/// @param wordsWritten the number of words successfully written to the transponder as int encoded in NSNumber or nil if none
/// @param moreAvailable YES if there are more transponders to be delivered
///
-(void)transponderReceivedFromWrite:(NSString *)epc crc:(NSNumber *)crc pc:(NSNumber *)pc rssi:(NSNumber *)rssi index:(NSNumber *)index accessErrorCode:(NSString *)aErrorCode backscatterErrorCode:(NSString *)bErrorCode wordsWritten:(NSNumber *)wordsWritten moreAvailable:(BOOL)moreAvailable;

@end

///
/// The block type to handle transponder information received
///
/// @param epc the transponder identifier as hex-encoded, ascii string
/// @param crc the transponder CRC as unsigned short encoded in NSNumber or nil if none
/// @param pc the transponder program control word as unsigned short encoded in NSNumber or nil if none
/// @param rssi the transponders RSSI value (in dBm) as int encoded in NSNumber or nil if none
/// @param index the index value as unsigned short encoded in NSNumber or nil if none
/// @param accessErrorCode the access error code as NSString or nil if none
/// @param backscatterErrorCode the backscatter error code as NSString or nil if none
/// @param wordsWritten the number of words successfully written to the transponder as int encoded in NSNumber or nil if none
/// @param moreAvailable YES if there are more transponders to be delivered
typedef void(^TSLWriteTransponderReceivedBlock)(NSString *epc, NSNumber *crc, NSNumber *pc, NSNumber *rssi, NSNumber *index, NSString *accessErrorCode, NSString *backscatterErrorCode,  NSNumber *wordsWritten, BOOL moreAvailable);


///
/// A command to write data to the memory banks of multiple transponders
///
@interface TSLWriteTransponderCommand : TSLTransponderMemoryCommandBase  <TSLQAlgorithmParametersProtocol, TSLQueryParametersProtocol>

/// @name Properties

/// When set to `TSL_TriState_YES` the Select operation is NOT performed
@property (nonatomic, readwrite) TSL_TriState inventoryOnly;

///
/// The write mode to use
///
/// Not all transponders support block write
///
@property (nonatomic, readwrite) TSL_WriteMode writeMode;

///
/// The block write mode to use when writing to supported transponders
/// (Impinj specific extension)
///
@property (nonatomic, readwrite) TSL_ImpinjBlockWriteMode impinjBlockWriteMode;

/// The TSLWriteSingleCommandTransponderReceivedDelegate to inform when each transponder is received
@property (weak, nonatomic, readwrite) id<TSLWriteCommandTransponderReceivedDelegate>transponderReceivedDelegate;

/// The Block invoked for each transponder received
@property (nonatomic, copy) TSLWriteTransponderReceivedBlock transponderReceivedBlock;

/// YES if any transponder responded to the write command
@property (nonatomic, readonly) BOOL didAnyTransponderRespond;


/// @name Factory Methods

///
/// Returns a synchronous TSLWriteTransponderCommand
/// (with self as synchronous responder)
///
+(TSLWriteTransponderCommand *)synchronousCommand;


/// @name Enum Helper methods

///
/// Test the given TSL_WriteMode for validity
///
///@param mode The TSL_WriteMode value to be tested
///@return YES if the given TSL_WriteMode is valid otherwise NO
///
+(BOOL)isValidWriteMode:(TSL_WriteMode)mode;

///
/// Parse the given parameter value string and determine the corresponding TSL_WriteMode
///
///@param value The parameter value as NSString
///@return The TSL_WriteMode corresponding to the given string. Invalid parameter values return TSL_WriteMode_NotSpecified
///
+(TSL_WriteMode)writeModeForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_WriteMode
///
///@param mode The TSL_WriteMode value to be converted
///@return The command parameter value for the given TSL_WriteMode or nil if invalid
///
+(NSString *)parameterValueForWriteMode:(TSL_WriteMode)mode;

///
/// Get the description corresponding to the given TSL_WriteMode
///
///@param mode The TSL_WriteMode value to be converted
///@return The user friendly name for the given TSL_WriteMode or nil if invalid
///
+(NSString *)descriptionForWriteMode:(TSL_WriteMode)mode;


///
/// Test the given TSL_ImpinjBlockWriteMode for validity
///
///@param mode The TSL_ImpinjBlockWriteMode value to be tested
///@return YES if the given TSL_ImpinjBlockWriteMode is valid otherwise NO
///
+(BOOL)isValidImpinjBlockWriteMode:(TSL_ImpinjBlockWriteMode)mode;

///
/// Parse the given parameter value string and determine the corresponding TSL_ImpinjBlockWriteMode
///
///@param value The parameter value as NSString
///@return The TSL_ImpinjBlockWriteMode corresponding to the given string. Invalid parameter values return TSL_ImpinjBlockWriteMode_NotSpecified
///
+(TSL_ImpinjBlockWriteMode)impinjBlockWriteModeForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_ImpinjBlockWriteMode
///
///@param mode The TSL_ImpinjBlockWriteMode value to be converted
///@return The command parameter value for the given TSL_ImpinjBlockWriteMode or nil if invalid
///
+(NSString *)parameterValueForImpinjBlockWriteMode:(TSL_ImpinjBlockWriteMode)mode;

///
/// Get the description corresponding to the given TSL_ImpinjBlockWriteMode
///
///@param mode The TSL_ImpinjBlockWriteMode value to be converted
///@return The user friendly name for the given TSL_ImpinjBlockWriteMode or nil if invalid
///
+(NSString *)descriptionForImpinjBlockWriteMode:(TSL_ImpinjBlockWriteMode)mode;




@end
