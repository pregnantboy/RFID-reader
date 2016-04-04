//
//  TSLViewController.m
//  Trigger
//
//  Created by Brian Painter on 11/09/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import "TSLSelectReaderViewController.h"

#import "TSLTriggerViewController.h"

//----------------------------------------------------------------------------------------------
//
// Trigger
//
// This is a simple App that connects to a paired TSL Reader and demonstrates how to use the
// device trigger to invoke application operations.
//
// This code shows the minimal code required to use the TSLAsciiCommand library. It has minimal
// error handling and does not necessarily implement all requirements of a well behaved iOS App
//
//
//----------------------------------------------------------------------------------------------

//
// When polling, the following refresh interval is used
//
static const NSTimeInterval kPollingInterval = 0.3;


@interface TSLTriggerViewController ()
{
    NSArray * _accessoryList;
    
    EAAccessory *_currentAccessory;
    TSLAsciiCommander *_commander;
    TSLSwitchResponder *_switchResponder;

    NSTimer *_pollingTimer;
}

@end

@implementation TSLTriggerViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
    
    
    // Create the TSLAsciiCommander used to communicate with the TSL Reader
    _commander = [[TSLAsciiCommander alloc] init];
    
    // TSLAsciiCommander requires TSLAsciiResponders to handle incoming reader responses
    
    // Add a logger to the commander to output all reader responses to the log file
    [_commander addResponder:[TSLLoggerResponder defaultResponder]];

    // Use the TSLSwitchResponder to monitor asynchronous switch notifications from the reader
    _switchResponder = [TSLSwitchResponder defaultResponder];
    // Register as the delegate for
    _switchResponder.switchStateReceivedDelegate = self;

    [_commander addResponder:_switchResponder];
    
    // Some synchronous commands will be used in the app
    [_commander addSynchronousResponder];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


-(void)viewWillAppear:(BOOL)animated
{
    // Listen for change in TSLAsciiCommander state
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(commanderChangedState) name:TSLCommanderStateChangedNotification object:_commander];
    
    // Update list of connected accessories
    _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Commander state change

-(void)commanderChangedState
{
    // Update the 'select reader' button
    if( !_commander.isConnected )
    {
        [self.selectReaderButton setTitle:@"Tap to select reader..." forState:UIControlStateNormal];
    }
}


#pragma mark - Reader Selection

//
// Segues for the controller
//
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if( [segue.identifier isEqualToString:@"segueSelectReader"] )
    {
        UINavigationController *navController = (UINavigationController *)segue.destinationViewController;
        TSLSelectReaderViewController *selectReaderViewcontroller = (TSLSelectReaderViewController *)navController.viewControllers[0];
        selectReaderViewcontroller.delegate = self;
    }
}


//
// The delegate for the SelectReaderViewController
//
-(void)didSelectReaderForRow:(NSInteger)row
{
    [self dismissViewControllerAnimated:YES completion:^{
        // Disconnect from the current reader, if any
        [_commander disconnect];
        
        // Connect to the chosen TSL Reader
        if( _accessoryList.count > 0 )
        {
            // The row is the offset into the list of connected accessories
            _currentAccessory = [_accessoryList objectAtIndex:row];
            [_commander connect:_currentAccessory];
        }
        
        // Prepare and show the connected reader
        [self initAndShowConnectedReader];
    }];
}


//
// Prepare the reader for use and display basic information about the reader
//
-(void)initAndShowConnectedReader
{
    if( _commander.isConnected )
    {
        // Display the serial number of the successfully connected unit
        [self.selectReaderButton setTitle:_currentAccessory.serialNumber forState:UIControlStateNormal];
        
        // Ensure the reader is in a known (default) state
        // No information is returned by the reset command
        TSLFactoryDefaultsCommand * resetCommand = [TSLFactoryDefaultsCommand synchronousCommand];
        [_commander executeCommand:resetCommand];
        
        // Notify user device has been reset
        if( resetCommand.isSuccessful )
        {
            self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:@"Reader reset to Factory Defaults\n"];
        }
        else
        {
            self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:@"!!! Unable to reset reader to Factory Defaults !!!\n"];
        }
        
        // Get version information for the reader
        // Use the TSLVersionInformationCommand synchronously as the returned information is needed below
        TSLVersionInformationCommand * versionCommand = [TSLVersionInformationCommand synchronousCommand];
        [_commander executeCommand:versionCommand];
        TSLBatteryStatusCommand *batteryCommand = [TSLBatteryStatusCommand synchronousCommand];
        [_commander executeCommand:batteryCommand];
        
        
        // Display some of the values obtained
        self.resultsTextView.text = [self.resultsTextView.text stringByAppendingFormat:@"\n%-16s %@\n%-16s %@\n%-16s %@\n%-16s %@\n\n",
                                     "Manufacturer:", versionCommand.manufacturer,
                                     "Serial Number:", versionCommand.serialNumber,
                                     "ASCII Protocl:", versionCommand.asciiProtocol,
                                     "Battery Level:", [NSString stringWithFormat:@"%d%%", batteryCommand.batteryLevel]];
        
        // Ensure new information is visible
        [self.resultsTextView scrollRangeToVisible:NSMakeRange(self.resultsTextView.text.length - 1, 1)];
        
    }
    else
    {
        [self.selectReaderButton setTitle:@"Tap to select reader..." forState:UIControlStateNormal];
    }
}


#pragma mark - Actions

-(void)pollTrigger
{
    @try
    {
        // Use the TSLSwitchStateCommand to query the trigger's current state
        TSLSwitchStateCommand *ssCommand = [TSLSwitchStateCommand synchronousCommand];
        [_commander executeCommand:ssCommand];
        
        [self updatePolledTriggerStatusView:ssCommand.state];
    }
    @catch (NSException *exception)
    {
        NSLog(@"Polling failed: %@", exception.reason);
    }
}

//
// Modify the switch actions based on the state of the reportsSwitch
//
- (IBAction)reportsSwitchChanged
{
    NSString *message = NULL;
    // Set the action of the device trigger using the TSLSwitchActionCommand
    TSLSwitchActionCommand *saCommand = [TSLSwitchActionCommand synchronousCommand];

    if( self.enableReportsSwitch.isOn )
    {
        // Disable the standard trigger actions and request switch status reports
        saCommand.asynchronousReportingEnabled = TSL_TriState_YES;
        saCommand.singlePressAction = TSL_SwitchAction_off;
        saCommand.doublePressAction = TSL_SwitchAction_off;

        message = @"\nReports are active\nThe device trigger status will be shown in the grey box above.\n";
    }
    else
    {
        // Only restore defaults if neither reports or polling are enabled
        if( !self.enablePollingSwitch.isOn )
        {
            // Restore the default settings
            saCommand.resetParameters = TSL_TriState_YES;
            
            message = @"\nDevice trigger actions reset to defaults.\n";
        }
        else
        {
            message = @"\nReports off\n";
            saCommand.asynchronousReportingEnabled = TSL_TriState_NO;
        }
    }

    @try
    {
        [_commander executeCommand:saCommand];
    }
    @catch (NSException *exception)
    {
        // If the command failed then revert the UI switch
        [self.enableReportsSwitch setOn:!self.enableReportsSwitch.isOn animated:YES];

        message = [NSString stringWithFormat:@"\nUnable to configure trigger reports:\n%@\n\n\n", exception.reason];
    }

    self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:message];

    // Ensure new messages are always visible
    [self.resultsTextView scrollRangeToVisible:NSMakeRange(self.resultsTextView.text.length - 1, 1)];

}


// Enable/disable the polling
- (IBAction)pollingSwitchChanged
{
    NSString *message = NULL;
    // Set the action of the device trigger using the TSLSwitchActionCommand
    TSLSwitchActionCommand *saCommand = [TSLSwitchActionCommand synchronousCommand];
    
    if( self.enablePollingSwitch.isOn )
    {
        // Disable the standard trigger actions and request switch status reports
        saCommand.singlePressAction = TSL_SwitchAction_off;
        saCommand.doublePressAction = TSL_SwitchAction_off;
        
        message = @"\nPolling is active\nThe device trigger status will be shown in the grey box above.\n";
        _pollingTimer = [NSTimer scheduledTimerWithTimeInterval:kPollingInterval target:self selector:@selector(pollTrigger) userInfo:nil repeats:YES];
    }
    else
    {
        [_pollingTimer invalidate   ];
        // Only restore defaults if neither reports or polling are enabled
        if( !self.enableReportsSwitch.isOn )
        {
            // Restore the default settings
            saCommand.resetParameters = TSL_TriState_YES;
            
            message = @"\nDevice trigger actions reset to defaults.\n";
        }
        else
        {
            message = @"\nPolling off\n";
        }
    }
    
    @try
    {
        [_commander executeCommand:saCommand];
    }
    @catch (NSException *exception)
    {
        [_pollingTimer invalidate   ];

        // If the command failed then revert the UI switch
        [self.enablePollingSwitch setOn:!self.enablePollingSwitch.isOn animated:YES];
        
        message = [NSString stringWithFormat:@"\nUnable to configure trigger polling:\n%@\n\n\n", exception.reason];
    }
    
    self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:message];
    
    // Ensure new messages are always visible
    [self.resultsTextView scrollRangeToVisible:NSMakeRange(self.resultsTextView.text.length - 1, 1)];
    
}


#pragma mark - TSLSwitchResponderStateReceivedDelegate delegate methods

-(void)switchStateReceived:(TSL_SwitchState)state
{
    // This is invoked on a non-UI thread so  dispatch changes to the UI thread to change the labels
    TSL_SwitchState newState = state;

    dispatch_async(dispatch_get_main_queue(), ^
                   {
                       [self updateTriggerStatusView:newState];
                   });
}


#pragma mark - Status Display

-(void)updateTriggerStatusView:(TSL_SwitchState)state
{
    switch( state )
    {
        case TSL_SwitchState_Off:
            self.singlePressLabel.hidden = YES;
            self.doublePressLabel.hidden = YES;
            break;
            
        case TSL_SwitchState_Single:
            self.singlePressLabel.hidden = NO;
            self.doublePressLabel.hidden = YES;
            break;
            
        case TSL_SwitchState_Double:
            self.singlePressLabel.hidden = YES;
            self.doublePressLabel.hidden = NO;
            break;
            
        default:
            break;
    }
}

-(void)updatePolledTriggerStatusView:(TSL_SwitchState)state
{
    switch( state )
    {
        case TSL_SwitchState_Off:
            self.singlePressPolledLabel.hidden = YES;
            self.doublePressPolledLabel.hidden = YES;
            break;
            
        case TSL_SwitchState_Single:
            self.singlePressPolledLabel.hidden = NO;
            self.doublePressPolledLabel.hidden = YES;
            break;
            
        case TSL_SwitchState_Double:
            self.singlePressPolledLabel.hidden = YES;
            self.doublePressPolledLabel.hidden = NO;
            break;
            
        default:
            break;
    }
}

@end
