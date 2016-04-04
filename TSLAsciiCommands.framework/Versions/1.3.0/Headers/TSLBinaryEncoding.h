//
//  TSLBinaryEncoding.h
//  AgoutiCommands
//
//  Created by Brian Painter on 09/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
///
/// Helper methods for handling ASCII Hex encoded values and converting to/from byte data
///
///
@interface TSLBinaryEncoding : NSObject

///
/// @name Testing Base 16 encoded NSStrings
///

///
/// Test a string for being an ASCII Hex encoded string representing a multiple word value
///
/// @param value The ASCII Hex value without 0x or 0X prefix
/// @return YES if the given value is a Hex number consisting of multiple words
///
+(BOOL)isValidWordAlignedHex: (NSString *)value;

///
/// @name Converting from Base 16 to NSData
///

///
/// Convert ASCII Hex encoded string to data bytes
///
/// @param string An NSString representing a whole number of Hex encoded Words with no prefix (e.g. 0x or 0X)
/// @return NSData object containing the bytes corresponding to the Hex values in the given string
///
/// @exception FormatException An NSException thrown when value is not in word aligned Hex format
///
+(NSData *)fromBase16String:(NSString *)string;


///
/// @name Converting from NSData to Base 16
///


///
/// Convert NSData into corresponding Hex digits and return as an NSString
///
/// @param value A non-nil NSData object containing the bytes to be converted
/// @return The ASCII Hex encoded representation of the NSData bytes as an NSString
///
/// @exception ArgumentNilException An NSException thrown when the value is nil
///
+(NSString *)toBase16String:(NSData *)value;

///
/// Convert NSData into corresponding Hex digits and return as an NSString
///
/// @param value An NSData object containing the bytes to be converted - can be nil
/// @return The ASCII Hex encoded representation of the NSData bytes as an NSString
///
/// @exception ArgumentNilException An NSException thrown when the value is nil
///
+(NSString *)toBase16StringSafe:(NSData *)value;

///
/// Convert a subset of NSData into corresponding Hex digits and return as an NSString
///
/// @param value A non-nil NSData object containing the bytes to be converted
/// @param offset The starting position within the NSData byte buffer of the bytes to be converted
/// @param length The number of bytes to be converted
/// @return The ASCII Hex encoded representation of the NSData bytes as an NSString
///
/// @exception ArgumentNilException An NSException thrown when the value is nil
/// @exception ArgumentOutOfRangeException An NSException thrown when the specified byte range is not fully within the NSData bytes
///
+(NSString *)toBase16String:(NSData *)value offset:(int)offset length:(int)length;

///
/// Convert a hex-encoded NSString to an int
///
/// @param string A non-nil ASCII Hex encoded NSString to be converted
/// @return The decimal value of the ASCII Hex encoded representation, or -1 if not valid hex, as an int
///
+(int)intFromBase16String:(NSString *)string;

#pragma mark - Data/String conversions


///
/// Convert the given data into an ASCII string
///
/// @param data An NSData object containing the bytes to be converted - can be nil
/// @return The ASCII encoded representation of the NSData bytes as an NSString
///
+(NSString *)asciiStringFromData:(NSData *)data;

///
/// Convert the ASCII string to an NSData object
///
/// @param string A non-nil ASCII encoded NSString to be converted
/// @return NSData object containing the bytes corresponding to the ASCII values in the given string
///
+(NSData *)dataFromAsciiString:(NSString *)string;



@end
