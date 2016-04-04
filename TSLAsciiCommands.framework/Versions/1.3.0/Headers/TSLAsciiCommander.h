//
//  TSLAsciiCommander.h
//  UHF RFID BT Reader Application
//
//  Created by Scrith on 24/09/2012.
//  Copyright (c) 2012 Scrith. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import "TSLAsciiCommandExecuting.h"
#import "TSLAsciiCommandExecutorBase.h"

///
/// Delegate protocol to specify supported devices based on their EAAccessory name and firmware revision
///
@protocol TSLAsciiCommanderSupportedDeviceDelegate <NSObject>

///
/// Test that a device with the given name and firmware version is supported
///
/// @param deviceName the name of the device as reported by the EAAccessory name property
/// @param firmwareVersion the firmware version as reported by the EAAccessory firmwareRevision property
///
/// @return YES if this app supports the given firmware version
///
+(BOOL)isSupportedDeviceForName:(NSString *)deviceName firmware:(NSString *)firmwareVersion;


@end


///
/// Notification sent when the ASCIICommander has changed state
///
/// userInfo != nil if commander was connected when notification issued
///
/// *Note:* This notification is always raised on the main application thread
///
extern NSString *TSLCommanderStateChangedNotification;


///
/// TSLAsciiCommander provides methods to communicate with TSL devices that use the TSL ASCII 2.0 Protocol
///
/// Instances of this class support:
///
/// - connection and disconnection from any TSL Reader paired to the iOS device
///   (including reconnection to that last sucessfully connected device)
/// - execution of a TSLAsciiCommand either synchronously or asynchronously
/// - management of the responder chain for data received from the TSL device
/// - execution of _raw_ TSL ASCII 2.0 commands e.g. `'.iv'`, `'.vr'` etc...
/// - provides notifications when the TSLAsciiCommander changes state e.g. connection/disconnection
///
/// *Important:* All communication with the TSL Device is performed on a separate thread.
/// This has the effect that all TSLAsciiResponders in the responder chain are executed off the main UI thread.
/// Several TSLAsciiCommands make use of delegates for custom processing e.g. the TSLInventoryCommand passes each
/// received transponder to its delegate. Any delegate should be aware that to modify any UI elements will require
/// explicit dispatch to the UI thread e.g. performSelectorOnMainThread:withObject:waitUntilDone: or via dispatch_async()
///
@interface TSLAsciiCommander : TSLAsciiCommandExecutorBase

/// @name Properties

/// The EAAccessory for the currently connected TSL Device
@property (strong, nonatomic) EAAccessory *connectedAccessory;

/// YES if the TSLAsciiCommander is connected to a TSL device
@property (nonatomic, readonly) BOOL isConnected;

/// YES if the last command completed as expected i.e. did not timeout
@property (nonatomic, readonly) BOOL isResponsive;

/// The time of the readers last activity (send or receive)
@property (atomic, readonly) NSDate *lastActivityTime;

///
/// The last command line sent
///
/// Because TSLAsciiCommands add a unique id number each time their commandLine property is accessed
/// this property can be used (primarily for debugging) to get the actual command line issued (if examined
/// immediately after the command is executed)
///
@property (nonatomic, readonly) NSString *lastCommandLine;

///
/// The delegate used to test for supported devices
/// 
@property (nonatomic, readwrite) Class <TSLAsciiCommanderSupportedDeviceDelegate> supportedDeviceDelegate;


/// @name Connecting to a TSL device

///
/// Connect the TSLAsciiCommander to the given TSL Device
///
/// This must be done before any TSLAsciiCommands are executed.
/// The serial number of the TSL device is stored in NSUserDefaults' standardUserDefaults upon successful connection.
///
/// @param accessory The TSL Device, as an EAAccessory, with which to connect to or nil.
///  If nil then a connection is attempted with the last, successfully connected device.
///
/// @return YES if successfully connected
///
-(BOOL)connect:(EAAccessory *)accessory;

/// Disconnects from the current device - this allows reconnections using connect:
-(void)disconnect;

///
/// Sends the signal to the accessory to permanently disconnect
///
/// Once issued this will require reconnecting to the reader to use the accessory again. This may require switching to the
/// iOS Settings App
///
/// This is a convenience method and is equivalent to sending a TSLSleepCommand to the reader
///
-(void)permanentlyDisconnect;


/// @name Communicating with the TSL device

///
/// Send the given string as a \<CR\>\<LF\> terminated string, to the TSL device
///
/// This method waits until the command has been successfuly sent
///
/// @param line The ASCII string to send to the device
/// @exception TSLInvalidOperationException thrown if no device is connected
///
-(void)send:(NSString *)line;


/// @name Instance Disposal

///
/// Stops the TSLAsciiCommander
///
/// When an instance of TSLAsciiCommander is no longer needed ensure that this method is called prior to object disposal
///
-(void) halt;

///
/// Aborts any currently running synchronous command
///
/// Note: The state of the aborted command is undefined.
///
-(void)abortSynchronousCommand;


@end
