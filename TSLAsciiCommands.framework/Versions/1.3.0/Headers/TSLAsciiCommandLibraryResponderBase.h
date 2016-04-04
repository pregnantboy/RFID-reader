//
//  TSLAsciiCommandLibraryResponderBase.h
//  AgoutiCommands
//
//  Created by Brian Painter on 18/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import "TSLAsciiCommandResponderBase.h"


///
/// Base class for library commands
///
/// This is identical to TSLAsciiCommandResponderBase except, by default, it will only
/// respond to commands containing the TSL_LibraryCommandId
///
@interface TSLAsciiCommandLibraryResponderBase : TSLAsciiCommandResponderBase

/// Enable/disable capture of non-library responses
@property BOOL captureNonLibraryResponses;

@end
