//
//  TSLAsciiCommandResponseNotifying.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 11/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

///
/// The Block type for response lifecycle event handlers
///
typedef void(^TSLCommandResponseLifecycleBlock)();



///
/// The protocol for notifying the command response lifecycle events
///
@protocol TSLAsciiCommandResponseNotifying <NSObject>

/// Invoked when the command response began
@property (nonatomic, copy) TSLCommandResponseLifecycleBlock responseBeganBlock;

/// Invoked when the command response ended
@property (nonatomic, copy) TSLCommandResponseLifecycleBlock responseEndedBlock;


@end
