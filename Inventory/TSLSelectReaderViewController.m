//
//  TSLSelectReaderViewController.m
//  Inventory
//
//  Created by Brian Painter on 28/08/2014.
//  Copyright (c) 2014 Technology Solutions (UK) Ltd. All rights reserved.
//
#import <ExternalAccessory/ExternalAccessory.h>
#import <ExternalAccessory/EAAccessoryManager.h>

#import "TSLSelectReaderViewController.h"

@interface TSLSelectReaderViewController ()
{
    NSArray * _accessoryList;
    NSInteger _selectedRow;
}

@property (weak, nonatomic) IBOutlet UIButton *doneButton;

@end

@implementation TSLSelectReaderViewController

+(NSString *)segueIdentifier
{
    return @"segueSelectReader";
}

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Listen for accessory connect/disconnects
    [[EAAccessoryManager sharedAccessoryManager] registerForLocalNotifications];
    
    _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
    _selectedRow = 0;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_accessoryDidConnect:) name:EAAccessoryDidConnectNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_accessoryDidDisconnect:) name:EAAccessoryDidDisconnectNotification object:nil];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return ( _accessoryList.count == 0 ) ? 1 : _accessoryList.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cellReader" forIndexPath:indexPath];

    if( _accessoryList.count == 0 )
    {
        cell.textLabel.text = @"No TSL devices connected!";
        cell.detailTextLabel.text = @"Use '+' button or Settings App to connect a reader";
    }
    else if( indexPath.row >= 0 )
    {
        EAAccessory * accessory = [_accessoryList objectAtIndex:indexPath.row];
        cell.textLabel.text = accessory.serialNumber;
        cell.detailTextLabel.text = [NSString stringWithFormat:@"FW: v%@    HW: v%@", accessory.firmwareRevision, accessory.hardwareRevision];
    }

    return cell;
}


#pragma mark - Table view delegate

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if( _accessoryList.count > 0 )
    {
        _selectedRow = indexPath.row;
        [self.delegate didSelectReaderForRow:_selectedRow];
    }
}


#pragma mark Keep the list of connected devices up-to-date

-(void) _accessoryDidConnect:(NSNotification *)notification
{
    EAAccessory *connectedAccessory = [[notification userInfo] objectForKey:EAAccessoryKey];
    // Only notify of change if the accessory added has valid protocol strings
    if( connectedAccessory.protocolStrings.count != 0 )
    {
        _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
        [self.tableView reloadData];
    }
}

- (void)_accessoryDidDisconnect:(NSNotification *)notification
{
//    EAAccessory *disconnectedAccessory = (EAAccessory *)[notification.userInfo objectForKey:@"EAAccessorySelectedKey"];

    _accessoryList = [[EAAccessoryManager sharedAccessoryManager] connectedAccessories];
    [self.tableView reloadData];
}


- (IBAction)doneAction:(id)sender
{
    [self.delegate didSelectReaderForRow:_selectedRow];
}

//
// Offer the user a dialog to select a new device to pair to
//
- (IBAction)pairToDevice:(UIBarButtonItem *)sender
{
    [[EAAccessoryManager sharedAccessoryManager] showBluetoothAccessoryPickerWithNameFilter:nil completion:^(NSError *error)
     {
         if( error == nil )
         {
             // Inform the user that the device is being connected
//             _hud = [TSLProgressHUD updateHUD:_hud inView:self.view forBusyState:YES withMessage:@"Waiting for device..."];
         }
         else
         {
             NSString *errorMessage = nil;
             switch (error.code)
             {
                 case EABluetoothAccessoryPickerAlreadyConnected:
                 {
                     NSLog(@"AlreadyConnected");
                     errorMessage = @"That device is already paired!\n\nTry again and wait a few seconds before choosing. Already paired devices will disappear from the list!";
                 }
                     break;
                     
                 case EABluetoothAccessoryPickerResultFailed:
                 case EABluetoothAccessoryPickerResultNotFound:
                     NSLog(@"NotFound");
                     errorMessage = @"Unable to find that device!\n\nEnsure the device is powered on and that the blue LED is flashing.";
                     break;
                     
                 case EABluetoothAccessoryPickerResultCancelled:
                     NSLog(@"Cancelled");
                     break;
                     
                 default:
                     break;
             }
             if( errorMessage )
             {
                 UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Pairing failed..."
                                                                 message:errorMessage
                                                                delegate:nil
                                                       cancelButtonTitle:@"OK"
                                                       otherButtonTitles:nil
                                       ];
                 [alert show];
             }
         }
     }];
}



@end
