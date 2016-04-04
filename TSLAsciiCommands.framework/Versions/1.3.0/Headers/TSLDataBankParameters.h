//
//  TSLDataBankParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 03/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLParametersProtocol.h"

///
/// Use macro to allow the creation of enum, parameter value and descriptive strings that are always in sync
///
#define TSL_FOR_EACH_DATA_BANK(select)\
\
select( TSL_DataBank_NotSpecified = 0,        @"",        @"Not specified"            )\
select( TSL_DataBank_ElectronicProductCode,   @"epc",     @"Electronic Product Code"  )\
select( TSL_DataBank_TransponderIdentifier,   @"tid",     @"Transponder Identifier"   )\
select( TSL_DataBank_User,                    @"usr",     @"User"                     )\
select( TSL_DataBank_Reserved,                @"res",     @"Reserved"                 )


///
/// The memory bank types
///
#define USE_ENUM(e,p,s) e,

typedef enum { TSL_FOR_EACH_DATA_BANK(USE_ENUM) } TSL_DataBank;

#undef USE_ENUM

///
/// Parameters related to Data Banks in commands and responses
///
@protocol TSLDataBankParametersProtocol <NSObject>

/// The transponder data bank to be used
@property (nonatomic, readwrite) TSL_DataBank bank;

/// The offset, in 16 bit words, from the start of the memory bank to where the data will be written
@property (nonatomic, readwrite) int offset;

/// Length in words of the data to write
@property (nonatomic, readwrite) int length;


/// The data read from or written to a transponder memory bank
@property (nonatomic, readwrite) NSData *data;

@end

///
/// Helper class for implementing TSLDataBankParametersProtocol
///
/// When issuing a command to the reader the values are interpreted as follows:
///
/// - `TSL_DataBank_NotSpecified` - _The value has not been specified and the corresponding parameter will not be sent_
/// - `TSL_DataBank_ElectronicProductCode` - _The value has been specified and is sent as `'epc'`_
/// - `TSL_DataBank_TransponderIdentifier` - _The value has been specified and is sent as `'tid'`_
/// - `TSL_DataBank_User` - _The value has been specified and is sent as `'usr'`_
/// - `TSL_DataBank_Reserved` - _The value has been specified and is sent as `'res'`_
///
/// When a command includes the '-p' option TSLAsciiCommand responses will set corresponding parameters as follows:
///
/// - `TSL_DataBank_ElectronicProductCode` - _The parameter is `'epc'`_
/// - `TSL_DataBank_TransponderIdentifier` - _The parameter is `'tid'`_
/// - `TSL_DataBank_User` - _The parameter is `'usr'`_
/// - `TSL_DataBank_Reserved` - _The parameter is `'res'`_
///
///
@interface TSLDataBankParameters : NSObject <TSLParametersProtocol>

///
/// Parse the given parameter value string and determine the corresponding TSL_DataBank
///
///@param value The parameter value as NSString
///@return The TSL_DataBank corresponding to the given string. Invalid parameter values return TSL_DataBank_NotSpecified
///
+(TSL_DataBank)bankForParameterValue:(NSString *)value;

///
/// Get the parameter value corresponding to the given TSL_DataBank
///
///@param bank The TSL_DataBank value to be converted
///@return The command parameter value for the given TSL_DataBank or nil if invalid
///
+(NSString *)parameterValueForBank:(TSL_DataBank)bank;


///
/// Get the description corresponding to the given TSL_DataBank
///
///@param bank The TSL_DataBank value to be converted
///@return The user friendly name for the given TSL_DataBank or nil if invalid
///
+(NSString *)descriptionForBank:(TSL_DataBank)bank;

///
/// Test the given TSL_DataBank for validity
///
///@param bank The TSL_DataBank value to be tested
///@return YES if the given TSL_DataBank is valid otherwise NO
///
+(BOOL)isValidBank:(TSL_DataBank)bank;

@end
