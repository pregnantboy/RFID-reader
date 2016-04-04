//
//  TSLTerminalResponder.h
//  TSLTerm
//
//  Created by Brian Painter on 22/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

@protocol TSLTerminalResponderDelegate <NSObject>

// The line received from the reader and indicator of additional future lines
-(void)receivedLine:(NSString *)line moreAvailable:(BOOL)moreAvailable;

@end

//
// Class to forward lines received from the reader
//
@interface TSLTerminalResponder : TSLAsciiCommandResponderBase

@property (nonatomic, readwrite) id<TSLTerminalResponderDelegate>delegate;

// Create a responder configured with the given delegate
+(TSLTerminalResponder *)defaultResponderWithDelegate:(id<TSLTerminalResponderDelegate>)delegate;

@end
