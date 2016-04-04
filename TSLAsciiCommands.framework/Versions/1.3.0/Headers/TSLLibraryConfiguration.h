//
//  TSLLibraryConfiguration.h
//  TSLAsciiCommands
//
//  Copyright (c) 2014-2015 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

///
/// Configuration values for the TSLAsciiCommand library
///
/// Use [TSLLibraryConfiguration sharedInstance] to access library properties
///
@interface TSLLibraryConfiguration : NSObject

/// Returns the library configuration
+(TSLLibraryConfiguration *)sharedInstance;

/// Control UTF8 support - off by default (introduced in SDK 1.2)
/// YES: data to and from the reader is treated as NSUTF8Encoding - if data is not valid UTF8 then it will be treated as NSASCIIEncoding
///  NO: data to and from the reader is treated as NSASCIIEncoding (this is identical to frameworks < v1.2)
@property (nonatomic, readwrite) bool useUTF8;


@end
