//
//  TSLViewController.m
//  Inventory
//
//  Created by Brian Painter on 15/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//
#import <ExternalAccessory/ExternalAccessory.h>

#import "TSLSelectReaderViewController.h"
#import "TSLInventoryViewController.h"



//----------------------------------------------------------------------------------------------
//
// Inventory
//
// This is a simple App that connects to a paired TSL Reader and performs an Inventory
//
// This code shows the minimal code required to use the TSLAsciiCommand library. It has minimal
// error handling and does not necessarily implement all requirements of a well behaved iOS App
//
//
//----------------------------------------------------------------------------------------------


@interface TSLInventoryViewController ()
{
    NSArray * _accessoryList;

    EAAccessory *_currentAccessory;
    TSLAsciiCommander *_commander;
    TSLInventoryCommand *_inventoryResponder;
    TSLBarcodeCommand *_barcodeResponder;

    int _transpondersSeen;
    NSString *_partialResultMessage;
    
    //For NSConnectionDelegate
    NSMutableData *_responseData;
    NSString *FORM_URL;

}

@property (nonatomic, readwrite) UIColor *defaultSelectReaderBackgroundColor;
@property (nonatomic, readwrite) UIColor *defaultSelectReaderTextColor;

@end

@implementation TSLInventoryViewController

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Create the TSLAsciiCommander used to communicate with the TSL Reader
    _commander = [[TSLAsciiCommander alloc] init];

    // TSLAsciiCommander requires TSLAsciiResponders to handle incoming reader responses

    // Add a logger to the commander to output all reader responses to the log file
    [_commander addResponder:[[TSLLoggerResponder alloc] init]];

    // Some synchronous commands will be used in the app
    [_commander addSynchronousResponder];

    
    // Performing an inventory could potentially take a long time if many transponders are in range so it is best to handle responses asynchronously
    //
    // The TSLInventoryCommand is a TSLAsciiResponder for inventory responses and can have a delegate
    // (id<TSLInventoryCommandTransponderReceivedDelegate>) that is informed of each transponder as it is received

    // Create a TSLInventoryCommand
    _inventoryResponder = [[TSLInventoryCommand alloc] init];
    
    // Add self as the transponder delegate
    _inventoryResponder.transponderReceivedDelegate = self;

    // Pulling the Reader trigger will generate inventory responses that are not from the library.
    // To ensure these are also seen requires explicitly requesting handling of non-library command responses
    _inventoryResponder.captureNonLibraryResponses = YES;

    // Remember the initial button colors
    self.defaultSelectReaderBackgroundColor = self.selectReaderButton.backgroundColor;
    self.defaultSelectReaderTextColor = self.selectReaderButton.titleLabel.textColor;

    //
    // Use the responseBeganBlock and responseEndedBlock to change the colour of the reader label while a response is being received
    //
    // Note: the weakSelf is used to avoid warning of retain cycles when self is used
    __weak typeof(self) weakSelf = self;

    _inventoryResponder.responseBeganBlock = ^
    {
        dispatch_async(dispatch_get_main_queue(),^
                       {
                           weakSelf.selectReaderButton.backgroundColor = [UIColor blueColor];
                           weakSelf.selectReaderButton.titleLabel.textColor = [UIColor whiteColor];
                       });
    };
    _inventoryResponder.responseEndedBlock = ^
    {
        dispatch_async(dispatch_get_main_queue(),^
                       {
                           weakSelf.selectReaderButton.backgroundColor = weakSelf.defaultSelectReaderBackgroundColor;
                           weakSelf.selectReaderButton.titleLabel.textColor = weakSelf.defaultSelectReaderTextColor;
                       });
    };

    // Add the inventory responder to the commander's responder chain
    [_commander addResponder:_inventoryResponder];

    //
    // Handling barcode responses is similar to the inventory
    //
    _barcodeResponder = [[TSLBarcodeCommand alloc] init];
    _barcodeResponder.barcodeReceivedDelegate = self;
    _barcodeResponder.captureNonLibraryResponses = YES;

    [_commander addResponder:_barcodeResponder];
    
    // No transponders seen yet
    _transpondersSeen = 0;

    _partialResultMessage = @"";
    
    FORM_URL = @"https://docs.google.com/forms/d/1KO7CJCceHMbylPzNntcu7OlzZApl2iqDsCqcI3jxTKg/formResponse";
}


- (void)viewDidUnload
{
    // Stop the TSLAsciiCommander
    [_commander halt];

    [self setResultsTextView:nil];
    [self setSelectReaderButton:nil];
    [super viewDidUnload];
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

#pragma mark - TSLInventoryCommandTransponderReceivedDelegate methods

//
// Each transponder received from the reader is passed to this method
//
// Parameters epc, crc, pc, and rssi may be nil
//
// Note: This is an asynchronous call from a separate thread
//
-(void)transponderReceived:(NSString *)epc crc:(NSNumber *)crc pc:(NSNumber *)pc rssi:(NSNumber *)rssi fastId:(NSData *)fastId moreAvailable:(BOOL)moreAvailable
{
    // Post to google sheet
    [self parseEpc:epc];
    
    // Append the transponder EPC identifier and RSSI to the results
    _partialResultMessage = [_partialResultMessage stringByAppendingFormat:@"%-28s  %4d\n", [epc UTF8String], [rssi intValue]];
    if( fastId != nil)
    {
        _partialResultMessage = [_partialResultMessage stringByAppendingFormat:@"%-6@  %@\n", @"TID:", [TSLBinaryEncoding toBase16String:fastId]];
    }
    
    _transpondersSeen++;
    
    // If this is the last transponder add a few blank lines
    if( !moreAvailable )
    {
        _partialResultMessage = [_partialResultMessage stringByAppendingFormat:@"\nTransponders seen: %4d\n\n", _transpondersSeen];
        _transpondersSeen = 0;
    }

    // This changes UI elements so perform it on the UI thread
    // Avoid sending too many screen updates as it can stall the display
    if( _transpondersSeen < 3 || _transpondersSeen % 10 == 0 )
    {
        [self performSelectorOnMainThread: @selector(updateResults:) withObject:_partialResultMessage waitUntilDone:NO];
        _partialResultMessage = @"";
    }
}

//EC31601112011001
#pragma mark - Google Form submission methods
-(void)parseEpc:(NSString *)epc {
    [self submitDataToFormWithEpc:epc
                           scanId:@"GantryScan"
                              uid:[epc substringWithRange:NSMakeRange(0, 8)]
                            block:[epc substringWithRange:NSMakeRange(8, 2)]
                            level:[epc substringWithRange:NSMakeRange(10, 2)]
                           weight:[NSString stringWithFormat:@"%@.%@",[epc substringWithRange:NSMakeRange(12, 2)],[epc substringWithRange:NSMakeRange(14, 2)]]
                           status:[self.statusSwitch titleForSegmentAtIndex:self.statusSwitch.selectedSegmentIndex]
     ];
}

-(void)submitDataToFormWithEpc:(NSString *)epc
                        scanId:(NSString *)scanId
                           uid:(NSString *)uid
                         block:(NSString *)block
                         level:(NSString *)level
                        weight:(NSString *)weight
                        status:(NSString *)status {
    
    // Declare POST request
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:FORM_URL]];
    
    // Set http method and Encoding
    [request setHTTPMethod:@"POST"];
    [request setValue:@"application/x-www-form-urlencoded; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    
    // Date formatter
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy"];
    NSString *year = [formatter stringFromDate:[NSDate date]];
    [formatter setDateFormat:@"MM"];
    NSString *month =[formatter stringFromDate:[NSDate date]];
    [formatter setDateFormat:@"dd"];
    NSString *day =[formatter stringFromDate:[NSDate date]];
    [formatter setDateFormat:@"HH"];
    NSString *hour =[formatter stringFromDate:[NSDate date]];
    [formatter setDateFormat:@"mm"];
    NSString *minute =[formatter stringFromDate:[NSDate date]];
    
    // Create Post Data
    NSString *postData = @"";
    
    postData = [postData stringByAppendingString:[@"entry.508208480_year=" stringByAppendingString:year]];
    postData = [postData stringByAppendingString:[@"&entry.508208480_month=" stringByAppendingString:month]];
    postData = [postData stringByAppendingString:[@"&entry.508208480_day=" stringByAppendingString:day]];
    postData = [postData stringByAppendingString:[@"&entry.207352404_hour=" stringByAppendingString:hour]];
    postData = [postData stringByAppendingString:[@"&entry.207352404_minute=" stringByAppendingString:minute]];
    postData = [postData stringByAppendingString:[@"&entry.1109226988=" stringByAppendingString:epc]];
    postData = [postData stringByAppendingString:[@"&entry.1583854227=" stringByAppendingString:scanId]];
    postData = [postData stringByAppendingString:[@"&entry.78262547=" stringByAppendingString:uid]];
    postData = [postData stringByAppendingString:[@"&entry.800935742=" stringByAppendingString:block]];
    postData = [postData stringByAppendingString:[@"&entry.626270335=" stringByAppendingString:level]];
    postData = [postData stringByAppendingString:[@"&entry.1948744252=" stringByAppendingString:weight]];
    postData = [postData stringByAppendingString:[@"&entry.1210691165=" stringByAppendingString:status]];
    
    // Append post data
    [request setHTTPBody:[postData dataUsingEncoding:NSUTF8StringEncoding]];
    
    NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
    [connection start];
}

- (IBAction)testPost:(id)sender {
    [self parseEpc:@"EC31601112011001"];
}

#pragma mark - NSConnectionDelegate methods
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
    // A response has been received, this is where we initialize the instance var you created
    // so that we can append data to it in the didReceiveData method
    // Furthermore, this method is called each time there is a redirect so reinitializing it
    // also serves to clear it
    _responseData = [[NSMutableData alloc] init];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    // Append the new data to the instance variable you declared
    [_responseData appendData:data];
}

- (NSCachedURLResponse *)connection:(NSURLConnection *)connection
                  willCacheResponse:(NSCachedURLResponse*)cachedResponse {
    // Return nil to indicate not necessary to store a cached response for this connection
    return nil;
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    // The request is complete and data has been received
    // You can parse the stuff in your instance variable now
    NSLog(@"Success!");
    self.success.alpha = 0;
    self.success.textColor = [UIColor colorWithRed:22/255.0 green:160/255.0 blue:33/255.0 alpha:1.0];

    self.success.text = @"Success";
    [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseIn
                     animations:^{ self.success.alpha = 1;}
                     completion:nil];
    [UIView animateWithDuration:0.5 delay:1.5 options:UIViewAnimationOptionCurveEaseIn
                     animations:^{ self.success.alpha = 0;}
                     completion:nil];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    // The request has failed for some reason!
    // Check the error var
    NSLog(@"Failed with error: %@", error);
    self.success.alpha = 0;
    self.success.textColor = [UIColor colorWithRed:192/255.0 green:57/255.0 blue:43/255.0 alpha:1.0];
    self.success.text = @"Fail";
    [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseIn
                     animations:^{ self.success.alpha = 1;}
                     completion:nil];
    [UIView animateWithDuration:0.5 delay:1.5 options:UIViewAnimationOptionCurveEaseIn
                     animations:^{ self.success.alpha = 0;}
                     completion:nil];
}

#pragma mark - TSLBarcodeCommandTransponderReceivedDelegate methods

//
// Each barcode received from the reader is passed to this method
//
// Note: This is an asynchronous call from a separate thread
//
-(void)barcodeReceived:(NSString *)data
{
    NSString *message = [NSString stringWithFormat:@"Barcode: %@\n", data];
    [self performSelectorOnMainThread: @selector(updateResults:) withObject:message waitUntilDone:NO];
}


#pragma mark

//
// Add the given message to the bottom of the results area
//
-(void)updateResults:(NSString *)message
{
    self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:message];

    // Ensure the end of the new information is visible
    [self.resultsTextView scrollRangeToVisible:NSMakeRange(self.resultsTextView.text.length - 1, 1)];
}


-(void)commanderChangedState
{
   // Update the 'select reader' button
   if( !_commander.isConnected )
   {
       [self.selectReaderButton setTitle:@"Tap to select reader..." forState:UIControlStateNormal];
   }
   else
   {
       [self setReaderOutputPower];
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

//
// Issue an asynchronous Inventory scan
//
- (IBAction)performInventory
{
    if( _commander.isConnected )
    {
        // Use the TSLInventoryCommand
        TSLInventoryCommand *invCommand = [[TSLInventoryCommand alloc] init];

        invCommand.includeTransponderRSSI = TSL_TriState_YES;

        // See if Impinj FastId is to be used
        invCommand.useFastId = self.fastIdSwitch.isOn ? TSL_TriState_YES : TSL_TriState_NO;

        int value = [self outputPowerFromSliderValue:self.outputPowerSlider.value];
        invCommand.outputPower = value;

        [_commander executeCommand:invCommand];
    }
}

//
// Issue a synchronous barcode scan
//
- (IBAction)performBarcodeScan:(id)sender
{
    if( _commander.isConnected )
    {
        // Use the TSLBarcodeCommand
        TSLBarcodeCommand *barCommand = [TSLBarcodeCommand synchronousCommand];
        barCommand.barcodeReceivedDelegate = self;
        
        [_commander executeCommand:barCommand];
    }
}

- (IBAction)clearResults
{
    self.resultsTextView.text = @"";
}


#pragma mark - Output Power Control

-(int)outputPowerFromSliderValue:(float)value
{
    int minPower = [TSLInventoryCommand minimumOutputPower];
    int maxPower = [TSLInventoryCommand maximumOutputPower];
    int range = maxPower - minPower;

    return (int)(value * range + minPower + 0.5);
}


- (IBAction)outputPowerChanged:(id)sender
{
    self.outputPowerLabel.text = [NSString stringWithFormat:@"%2d", [self outputPowerFromSliderValue:self.outputPowerSlider.value]];
}

- (IBAction)outputPowerEditingDidEnd:(id)sender
{
    int value = [self outputPowerFromSliderValue:self.outputPowerSlider.value];
    self.resultsTextView.text = [self.resultsTextView.text stringByAppendingFormat:@"Output level changed to: %2d\n\n", value];
    [self setReaderOutputPower];
}

-(void)setReaderOutputPower
{
    if( _commander.isConnected )
    {
        int value = [self outputPowerFromSliderValue:self.outputPowerSlider.value];
        TSLInventoryCommand *command = [TSLInventoryCommand synchronousCommand];
        command.takeNoAction = TSL_TriState_YES;
        command.outputPower = value;
        [_commander executeCommand:command];
    }
}

@end
