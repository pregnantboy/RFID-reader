//
//  TSLTerminalResponder.m
//  TSLTerm
//
//  Created by Brian Painter on 22/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import "TSLTerminalResponder.h"

@implementation TSLTerminalResponder

@synthesize delegate;


+(TSLTerminalResponder *)defaultResponderWithDelegate:(id<TSLTerminalResponderDelegate>)delegate
{
    TSLTerminalResponder *tr = [[TSLTerminalResponder alloc] init];
    tr.delegate = delegate;
    return tr;
}



//
// Each correctly terminated line from the device is passed to this method for processing
//
// @return YES if this line should not be passed to any other responder
//
-(BOOL)processReceivedLine:(NSString *)fullLine header:(NSString *)header value:(NSString *)value moreLinesAvailable:(BOOL) moreAvailable
{
    // Output everything seen to the delegate
    [self.delegate receivedLine:fullLine moreAvailable:moreAvailable];

    return NO;
}

@end
