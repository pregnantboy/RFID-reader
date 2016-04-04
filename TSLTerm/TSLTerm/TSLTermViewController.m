//
//  TSLViewController.m
//  TSLTerm
//
//  Created by Brian Painter on 22/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//


#import <TSLAsciiCommands/TSLAsciiCommands.h>

#import "TSLAppDelegate.h"
#import "TSLScrollingTranscriptCell.h"
#import "TSLTerminalResponder.h"

#import "TSLTermViewController.h"


//----------------------------------------------------------------------------------------------
//
// TSLTerm
//
// This is a simple App that connects to a paired TSL Reader and allows user to issue raw
// ASCII 2.0 commands to the reader and see the responses
//
// This code has minimal error handling and does not necessarily implement all requirements
// of a well behaved iOS App
//
//----------------------------------------------------------------------------------------------


@interface TSLTermViewController ()
{
    NSArray * _accessoryList;
   
    TSLAppDelegate * _appDelegate;
    TSLAsciiCommander * _commander;

    EAAccessory *_currentAccessory;
    TSLTerminalResponder *_terminalResponder;

    NSMutableArray *_transcript;
}

@property (weak, nonatomic) IBOutlet UIButton *selectReaderButton;
@property (strong, nonatomic) IBOutlet UIToolbar *dataInputToolbar;

@end

@implementation TSLTermViewController

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
    return YES;
}


- (void)viewDidLoad
{
    [super viewDidLoad];

    // Get the single commander used for this application
    _appDelegate = [UIApplication sharedApplication ].delegate;
    _commander = _appDelegate.commander;
    
    
    self.transcriptTableView.dataSource = self;

    _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
    

    // Connect to the chosen TSL Reader
    _currentAccessory = _commander.connectedAccessory;
    
    // Report the connection status
    [self.selectReaderButton setTitle:_commander.isConnected ? _currentAccessory.serialNumber : @"Tap to select reader..." forState:UIControlStateNormal];
    
    // Add terminal responder to forward all lines to the supplied delegate
    _terminalResponder = [TSLTerminalResponder defaultResponderWithDelegate:self];
    [_commander addResponder:_terminalResponder];

    // Create an array to hold the sent and received messages
    _transcript = [NSMutableArray arrayWithCapacity:300];
    [self clearTranscript:nil];

    // Provide a toolbar to ease input of '.' and to issue commands with keyboard visible
    self.commandTextField.inputAccessoryView = self.dataInputToolbar;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)viewDidUnload
{
    // Stop the TSLAsciiCommander
    [_commander halt];
    
    [self setCommandTextField:nil];
    [self setTranscriptTableView:nil];
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



#pragma mark Respond to the TSLAsciiCommander changing state

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
    }
    else
    {
        // Display the user instructions
        [self.selectReaderButton setTitle:@"Tap to select reader..." forState:UIControlStateNormal];
    }
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _transcript.count;
}


//
// Returns a cell with a horizontally scrolling label containing the given text
//
-(UITableViewCell *)scrollingTranscriptCellWithIdentifier:(NSString *)identifier Text:(NSString *)text
{
    TSLScrollingTranscriptCell *scrollCell = [self.transcriptTableView dequeueReusableCellWithIdentifier:identifier];
    
    scrollCell.label.text = text;
    CGFloat textWidth = [scrollCell.label.text sizeWithFont:[scrollCell.label font]].width + scrollCell.label.frame.origin.x + 16;
    CGRect currentRect = scrollCell.label.frame;
    CGRect frame = CGRectMake(currentRect.origin.x, currentRect.origin.y, textWidth, currentRect.size.height);
    scrollCell.label.frame = frame;
    
    // Set the content size of the scroll view to just hold the text
    [scrollCell.scrollView setContentSize:CGSizeMake(textWidth, scrollCell.scrollView.frame.size.height)];
    
    // Use the created cell
    return scrollCell;
}

- (UITableViewCell *)tableView:(UITableView *)theTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    // All sent commands begin with the '.'
    NSString *cellText = [_transcript objectAtIndex:indexPath.row];

    // Assume text is received text
    NSString *cellIdentifier = [cellText hasPrefix:@"."] ? @"sendCell" : @"receiveCell";

 
    UITableViewCell *cell = [self scrollingTranscriptCellWithIdentifier:cellIdentifier Text:cellText ];

    return cell;
}


#pragma mark - TSLTerminalResponder Delegate

-(void)receivedLine:(NSString *)line moreAvailable:(BOOL)moreAvailable
{
    // Add the line to the transcript and update display
    [_transcript addObject:line];
    [self.transcriptTableView reloadData];
    [self.transcriptTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:_transcript.count - 1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:!moreAvailable];
//    NSLog(@"More: %d", moreAvailable);
}



#pragma mark - Actions

- (IBAction)insertDot:(id)sender
{
    [self.commandTextField insertText:@"."];
}

- (IBAction)insertDash:(id)sender
{
    [self.commandTextField insertText:@"-"];
}

- (IBAction)dismissKeyboardAndSendCommand
{
    [self.commandTextField resignFirstResponder];
    [self sendCommand];
}

- (IBAction)sendCommand
{

    if( !_commander.isConnected )
    {
        // Show error alert
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle: @"No Reader Connected"
                              message: @"Connect a reader and try again!"
                              delegate: nil
                              cancelButtonTitle:@"OK"
                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    NSString *command = [self.commandTextField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

    if( ![command hasPrefix:@"."] )
    {
        // Show error alert
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle: @"Bad command"
                              message: @"Commands must begin with '.'"
                              delegate: nil
                              cancelButtonTitle:@"OK"
                              otherButtonTitles:nil];
        [alert show];
    }
    else
    {
        // Send the command to the reader
        [_commander send:command];

        // Add the command to the transcript and update display
        [_transcript addObject:command];
        [self.transcriptTableView reloadData];
    }
}


// Remove all entries from the transcript
- (IBAction)clearTranscript:(id)sender
{
    [_transcript removeAllObjects];
    // Add empty lines to make new commands appear at the bottom of the view
    for(int i=0; i < 30; ++i )
    {
        [_transcript addObject:@""];
    }
    [self receivedLine:@"" moreAvailable:NO];
}


@end
