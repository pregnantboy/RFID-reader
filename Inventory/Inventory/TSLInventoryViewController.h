//
//  TSLInventoryViewController.h
//  Inventory
//
//  Created by Brian Painter on 15/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <TSLAsciiCommands/TSLAsciiCommands.h>

#import "TSLSelectReaderProtocol.h"

@interface TSLInventoryViewController : UIViewController<TSLInventoryCommandTransponderReceivedDelegate, TSLBarcodeCommandBarcodeReceivedDelegate, TSLSelectReaderProtocol, NSURLConnectionDelegate>

// The results of an inventory will be displayed here
@property (weak, nonatomic) IBOutlet UITextView *resultsTextView;

// The current antenna output power
@property (weak, nonatomic) IBOutlet UILabel *outputPowerLabel;
@property (weak, nonatomic) IBOutlet UISlider *outputPowerSlider;

// This button allows the user to enable Impinj FastId support
@property (weak, nonatomic) IBOutlet UISwitch *fastIdSwitch;

// Button used for changing and displaying connected reader
@property (weak, nonatomic) IBOutlet UIButton *selectReaderButton;
@property (weak, nonatomic) IBOutlet UILabel *success;
@property (weak, nonatomic) IBOutlet UISegmentedControl *statusSwitch;

// Instructs the reader to perform an inventory
- (IBAction)performInventory;
- (IBAction)performBarcodeScan:(id)sender;

// Clear the results area
- (IBAction)clearResults;
@end
