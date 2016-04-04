//
//  "TSLConstants.h"
//  AgoutiCommands
//
//  Created by Scrith on 14/09/2012.
//  Copyright (c) 2012 Technology Solutions UK Ltd. All rights reserved.
//

// These are the limits of the changes that can be made to the power output
#define TSL_MIN_CARRIER_POWER 10
#define TSL_MAX_CARRIER_POWER 29

/// This string is inserted into every command issued by the library
extern NSString * const TSL_LibraryCommandId;

/// This is the only external accessory protocol supported by this library
extern NSString * const TSL_supportedAccessoryProtocol;

/// The key for the last accessory used by the app
extern NSString * const TSL_lastAccessorySerialNumberKey;
