//
//  TSLLicenceKeyViewController.m
//  Licence Key
//
//  Created by Brian Painter on 02/09/2014.
//  Copyright (c) 2014 Technology Solutions (UK) Ltd. All rights reserved.
//
#import <ExternalAccessory/ExternalAccessory.h>
#import <CommonCrypto/CommonDigest.h>

#import "TSLSelectReaderViewController.h"

#import "TSLLicenceKeyViewController.h"


// This is the key for storing the secret value
#define SECRET_VALUE_KEY        @"secretKey"
#define DEFAULT_SECRET_VALUE    @"Setec Astronomy"

// The licence key command was introduced in ASCII protocl 2.2
#define MINIMUM_ASCII_PROTOCOL_VERSION_FOR_LICENCE_KEY_COMMAND  @"2.2"

@interface TSLLicenceKeyViewController ()
{
    NSArray * _accessoryList;
    
    EAAccessory *_currentAccessory;
    TSLAsciiCommander *_commander;
    TSLInventoryCommand *_inventoryResponder;
    TSLBarcodeCommand *_barcodeResponder;
    
    int _transpondersSeen;
    NSString *_partialResultMessage;
}

@property (nonatomic, readwrite) UIColor *defaultSelectReaderBackgroundColor;
@property (nonatomic, readwrite) UIColor *defaultSelectReaderTextColor;

@property (weak, nonatomic) IBOutlet UIButton *selectReaderButton;

@property (weak, nonatomic) IBOutlet UILabel *authorisationLabel;
@property (weak, nonatomic) IBOutlet UITextField *secretValueTextField;

@property (weak, nonatomic) IBOutlet UISwitch *onlyRespondToAuthorisedReadersSwitch;

@property (weak, nonatomic) IBOutlet UIButton *inventoryButton;
@property (weak, nonatomic) IBOutlet UIButton *barcodeButton;

@property (weak, nonatomic) IBOutlet UITextView *resultsTextView;

@property (nonatomic, readwrite) BOOL readerIsAuthorised;
@property (nonatomic, readwrite) BOOL onlyUseAuthorisedReader;

@property (nonatomic, readwrite) TSLVersionInformationCommand *versionInformationCommand;

@end

@implementation TSLLicenceKeyViewController

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
        if( weakSelf.readerIsAuthorised || !weakSelf.onlyUseAuthorisedReader )
        {
            dispatch_async(dispatch_get_main_queue(),^
                           {
                               weakSelf.selectReaderButton.backgroundColor = [UIColor blueColor];
                               weakSelf.selectReaderButton.titleLabel.textColor = [UIColor whiteColor];
                           });
        }
    };
    _inventoryResponder.responseEndedBlock = ^
    {
        if( weakSelf.readerIsAuthorised || !weakSelf.onlyUseAuthorisedReader )
        {
            dispatch_async(dispatch_get_main_queue(),^
                           {
                               weakSelf.selectReaderButton.backgroundColor = weakSelf.defaultSelectReaderBackgroundColor;
                               weakSelf.selectReaderButton.titleLabel.textColor = weakSelf.defaultSelectReaderTextColor;
                           });
        }
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

    // Default to only using authorised readers
    self.onlyUseAuthorisedReader = YES;

    // Check for a stored "secret" value
    NSString *storedSecretValue = [[NSUserDefaults standardUserDefaults] objectForKey:SECRET_VALUE_KEY];
    if( storedSecretValue == nil )
    {
        storedSecretValue = DEFAULT_SECRET_VALUE;
        [[NSUserDefaults standardUserDefaults] setObject:storedSecretValue forKey:SECRET_VALUE_KEY];
    }

    // Set the starting value for the "secret" text field
    self.secretValueTextField.text = storedSecretValue;

    // Create the version information command
    self.versionInformationCommand = [TSLVersionInformationCommand synchronousCommand];
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

    [self.onlyRespondToAuthorisedReadersSwitch setOn: self.onlyUseAuthorisedReader];
    [self updateUIState];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Reader selection

-(void)commanderChangedState
{
    // Report the connection status
    [self.selectReaderButton setTitle:_commander.isConnected ? _currentAccessory.serialNumber : @"Tap to select reader..." forState:UIControlStateNormal];
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
        [self clearResults:nil];
        [self initAndShowConnectedReader];
    }];
}


//
// Convert the given version string into an int so that
// version numbers can be compared
// Returns -1 if the version string is invalid
//
+(int)comparableVersionValue:(NSString *)versionString
{
    NSArray *parts = [versionString componentsSeparatedByString:@"."];
    if( parts.count == 0 || parts.count > 3 ) return -1;
    
    int scale = 1 << 16;
    int value = 0;
    for( NSString *part in parts )
    {
        int digitValue = [part intValue];
        value += digitValue * scale;
        scale >>= 8;
    }
    return value;
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

        // Update the version information for the connected reader
        [_commander executeCommand:self.versionInformationCommand];

        if( [self.class comparableVersionValue:self.versionInformationCommand.asciiProtocol] < [self.class comparableVersionValue:MINIMUM_ASCII_PROTOCOL_VERSION_FOR_LICENCE_KEY_COMMAND] )
        {
            [self updateResults:[NSString stringWithFormat:@"Reader does not support licence keys\nRequires ASCII protocol: %@\nReader ASCII protocol: %@\n",
                                 MINIMUM_ASCII_PROTOCOL_VERSION_FOR_LICENCE_KEY_COMMAND,
                                 self.versionInformationCommand.asciiProtocol
                                 ]];
        }
        [self validateReader];
    }
    else
    {
        [self.selectReaderButton setTitle:@"Tap to select reader..." forState:UIControlStateNormal];
        [self updateUIState];
    }
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
    if( self.readerIsAuthorised || !self.onlyUseAuthorisedReader )
    {
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
}

#pragma mark - TSLBarcodeCommandTransponderReceivedDelegate methods

//
// Each barcode received from the reader is passed to this method
//
// Note: This is an asynchronous call from a separate thread
//
-(void)barcodeReceived:(NSString *)data
{
    if( self.readerIsAuthorised || !self.onlyUseAuthorisedReader )
    {
        NSString *message = [NSString stringWithFormat:@"Barcode: %@\n", data];
        [self performSelectorOnMainThread: @selector(updateResults:) withObject:message waitUntilDone:NO];
    }
}

#pragma mark - Display Updates

//
// Add the given message to the bottom of the results area
//
-(void)updateResults:(NSString *)message
{
    self.resultsTextView.text = [self.resultsTextView.text stringByAppendingString:message];
    
    // Ensure the end of the new information is visible
    [self.resultsTextView scrollRangeToVisible:NSMakeRange(self.resultsTextView.text.length - 1, 1)];
}


//
// Configure the state of the controls
//
-(void)updateUIState
{
    // Set the appearance
    self.authorisationLabel.text = self.readerIsAuthorised ? @"Authorised" : @"Not Authorised";
    self.authorisationLabel.backgroundColor = self.readerIsAuthorised ? [UIColor colorWithRed:0.0 green:0.7 blue:0.0 alpha:1.0] : [UIColor grayColor];
}


#pragma mark - UITextField Delegate methods


// Dismiss the keyboard when return pressed
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}


-(void)textFieldDidEndEditing:(UITextField *)textField
{
    // "Secret" has changed so revalidate the reader
    if(_commander.isConnected )
    {
        [self validateReader];
    }
}


#pragma mark Actions

- (IBAction)secretChanged:(id)sender
{
    NSString *newSecret = [self.secretValueTextField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    [[NSUserDefaults standardUserDefaults] setObject:newSecret forKey:SECRET_VALUE_KEY];
    if( _commander.isConnected )
    {
        [self validateReader];
    }
}

- (IBAction)authoriseReader:(id)sender
{
    [self clearResults:nil];

    if( _commander.isConnected)
    {
        // Calculate the licence key based on the reader and the current value of the secret
        NSString *secretValue = [self.secretValueTextField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        NSString *licenceKey = [self createLicenceKeyForSerialNumber:self.versionInformationCommand.serialNumber
                                                    bluetoothAddress:self.versionInformationCommand.bluetoothAddress
                                                              secret:secretValue];
        
        // Set the licence key on the reader
        TSLLicenceKeyCommand *licenceKeyCommand = [TSLLicenceKeyCommand synchronousCommand];
        licenceKeyCommand.licenceKey = licenceKey;
        licenceKeyCommand.deleteLicenceKey = TSL_DeleteConfirmation_YES;
        [_commander executeCommand:licenceKeyCommand];
        
        if( licenceKeyCommand.isSuccessful )
        {
            // Verify that the
            [self validateReader];
        }
        else
        {
            [self updateResults:@"Unable to authorise reader\n"];
            [self updateUIState];
        }
    }
    else
    {
        [self updateResults:@"Reader not connected\n"];
    }
}


- (IBAction)deauthoriseReader:(id)sender
{
    [self clearResults:nil];
    
    if( _commander.isConnected)
    {
        // Delete the licence key on the reader
        TSLLicenceKeyCommand *licenceKeyCommand = [TSLLicenceKeyCommand synchronousCommand];
        licenceKeyCommand.deleteLicenceKey = TSL_DeleteConfirmation_YES;
        [_commander executeCommand:licenceKeyCommand];
        
        if( licenceKeyCommand.isSuccessful )
        {
            // Verify that the
            [self validateReader];
        }
        else
        {
            [self updateResults:@"Unable to de-authorise reader\n"];
            [self updateUIState];
        }
    }
    else
    {
        [self updateResults:@"Reader not connected\n"];
    }
}

- (IBAction)applicationResponseSwitchChanged:(id)sender
{
    self.onlyUseAuthorisedReader = self.onlyRespondToAuthorisedReadersSwitch.isOn;
}

//
// Issue an asynchronous Inventory scan
//
- (IBAction)performInventoryScan:(id)sender
{
    if( self.readerIsAuthorised || !self.onlyUseAuthorisedReader)
    {
        if( _commander.isConnected)
        {
            // Use the TSLInventoryCommand
            TSLInventoryCommand *invCommand = [[TSLInventoryCommand alloc] init];
            
            invCommand.includeTransponderRSSI = TSL_TriState_YES;
            
            [_commander executeCommand:invCommand];
        }
        else
        {
            [self updateResults:@"Reader not connected\n"];
        }
    }
    else
    {
        [self updateResults:@"Reader NOT authorised\n"];
    }
}

//
// Issue a synchronous barcode scan
//
- (IBAction)performBarcodeScan:(id)sender
{
    if( self.readerIsAuthorised || !self.onlyUseAuthorisedReader)
    {
        if( _commander.isConnected)
        {
            // Use the TSLBarcodeCommand
            TSLBarcodeCommand *barCommand = [TSLBarcodeCommand synchronousCommand];
            barCommand.barcodeReceivedDelegate = self;
            
            [_commander executeCommand:barCommand];
        }
        else
        {
            [self updateResults:@"Reader not connected\n"];
        }
    }
    else
    {
        [self updateResults:@"Reader NOT authorised\n"];
    }
}

- (IBAction)clearResults:(id)sender
{
    self.resultsTextView.text = @"";
}


#pragma  mark - Reader Validation

//
// Convert the given NSString into a SHA-256 hash represented as ASCII encoded hex digits
//
+ (NSString *)asciiHexEncodedSHA256Hash:(NSString *)input
{
    const char *cstr = [input cStringUsingEncoding:NSUTF8StringEncoding];
    NSData *data = [NSData dataWithBytes:cstr length:input.length];
    uint8_t digest[CC_SHA256_DIGEST_LENGTH];
    
    // It takes in the data, how much data, and then output format, which in this case is an int array.
    CC_SHA256(data.bytes, data.length, digest);
    
    NSMutableString* output = [NSMutableString stringWithCapacity:CC_SHA256_DIGEST_LENGTH * 2];
    
    // Parse through the CC_SHA256 results (stored inside of digest[]).
    for(int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++) {
        [output appendFormat:@"%02x", digest[i]];
    }
    NSLog( @"HashLength: %d", output.length);
    return output;
}


//
// Returns the licence key corresponding to the connected reader and the current "secret" value
//
-(NSString *)createLicenceKeyForSerialNumber:(NSString *)serialNumber bluetoothAddress:(NSString *)bluetoothAddress secret:(NSString *)secretValue
{
    NSString *licenceKey = nil;
    NSString *licenceKeySourceValue = [serialNumber stringByAppendingFormat:@"%@%@", bluetoothAddress, secretValue];
//    NSString *licenceKeySourceValue = @"ABCDEFGHIJKLMNOP";

    //Apply Crypto hash here
//    licenceKey = licenceKeySourceValue;
    licenceKey = [self.class asciiHexEncodedSHA256Hash:licenceKeySourceValue];

    return licenceKey;
}


//
// Checks to see if the reader contains a valid licence key
//
-(void)validateReader
{
    BOOL isAuthorisedReader = NO;

    NSString *secretValue = [self.secretValueTextField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];

    // Retrieve the current licence key
    TSLLicenceKeyCommand *licenceKeyCommand = [TSLLicenceKeyCommand synchronousCommand];
    [_commander executeCommand:licenceKeyCommand];
    
    if( licenceKeyCommand.isSuccessful )
    {
        [self updateResults:[NSString stringWithFormat:@"Reader Licence key:\n%@\n\n", licenceKeyCommand.licenceKey]];

        // Calculate the expected licence key based on the reader and the current value of the secret
        NSString *expectedLicenceKey = [self createLicenceKeyForSerialNumber:self.versionInformationCommand.serialNumber bluetoothAddress:self.versionInformationCommand.bluetoothAddress secret:secretValue];
        [self updateResults:[NSString stringWithFormat:@"Expected Licence key:\n%@\n\n", expectedLicenceKey]];

        isAuthorisedReader = [licenceKeyCommand.licenceKey isEqualToString:expectedLicenceKey];
    }
    else
    {
        [self updateResults:@"Unable to validate reader\n"];
    }

    self.readerIsAuthorised = isAuthorisedReader;
    [self updateUIState];
}

@end
