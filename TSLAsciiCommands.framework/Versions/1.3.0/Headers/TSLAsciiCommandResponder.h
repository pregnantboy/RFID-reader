//
//  TSLAsciiCommandResponder.h
//  AgoutiCommands
//
//  Created by Brian Painter on 13/03/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

// Forward reference
@protocol TSLAsciiCommandResponderDelegate;


///
/// The interface for classes that handle responses from ASCII 2.0 commands
///
@protocol TSLAsciiCommandResponder <NSObject>


///@name Properties

/// Gets the response as received from the device as NSArray of NSStrings
@property (nonatomic, readonly) NSArray *response;

/// Gets a value indicating whether the command executed successfully
@property (nonatomic, readonly) BOOL isSuccessful;

/// Gets the error code or null if none
@property (nonatomic, readonly) NSString *errorCode;

/// Gets the messages received from the device as NSArray of NSStrings or nil if none
@property (nonatomic, readonly) NSArray *messages;

/// Gets the parameters received from the device as NSArray of NSStrings or nil if none
@property (nonatomic, readonly) NSArray *parameters;

/// Gets/sets the delegate to be called to handle synchronous command messages
@property (weak, nonatomic, readwrite) id<TSLAsciiCommandResponderDelegate> synchronousCommandDelegate;


///@name Methods

///
/// Each correctly terminated line from the device is passed to this method for processing
///
///
/// @param fullLine The line to be processed
/// @param moreAvailable When YES indictates there are additional lines to be processed (and will also be passed to this method)
///
/// @return YES if this line should not be passed to any other responder
///
-(BOOL)processReceivedLine:(NSString *)fullLine moreLinesAvailable:(BOOL) moreAvailable;

///
/// Clears the values from the last response
///
/// Derived classes must call super class to ensure correct operation
///
-(void)clearLastResponse;

///
/// Reset the responder to its default state
///
/// Derived classes must call super class to ensure correct operation
///
-(void)reset;


@end
