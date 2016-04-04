//
//  TSLTransponderParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 03/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLTriState.h"
#import "TSLParametersProtocol.h"

///
/// Parameters related to the Transponder information in command responses
///
/// TSL_TriState values default to TSL_TriState_NOT_SPECIFIED
/// 
@protocol TSLTransponderParametersProtocol <NSObject>


/// Control the inclusion of checksum information in reader responses
@property (nonatomic, readwrite) TSL_TriState includeChecksum;

/// Control the inclusion of the EPC PC value in reader responses
@property (nonatomic, readwrite) TSL_TriState includePC;

/// Control the inclusion of RSSI value in reader responses
@property (nonatomic, readwrite) TSL_TriState includeTransponderRSSI;

/// Control the inclusion of index numbers for multiple values in reader responses
@property (nonatomic, readwrite) TSL_TriState includeIndex;

/// The password required to access transponders - defaults to nil
@property (nonatomic, readwrite) NSString *accessPassword;

@end


///
/// Helper class for implementing TSLTransponderParametersProtocol
///
@interface TSLTransponderParameters : NSObject <TSLParametersProtocol>

@end
