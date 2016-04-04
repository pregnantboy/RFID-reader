//
//  TSLViewController.h
//  Trigger
//
//  Created by Brian Painter on 11/09/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <TSLAsciiCommands/TSLSwitchResponder.h>

#import "TSLSelectReaderProtocol.h"

@interface TSLTriggerViewController : UIViewController<UIPickerViewDelegate, TSLSwitchResponderStateReceivedDelegate, TSLSelectReaderProtocol>

// Additional information will be displayed here
@property (weak, nonatomic) IBOutlet UITextView *resultsTextView;

// This button allows the user to select a reader
@property (weak, nonatomic) IBOutlet UIButton *selectReaderButton;

@property (weak, nonatomic) IBOutlet UISwitch *enableReportsSwitch;
@property (weak, nonatomic) IBOutlet UISwitch *enablePollingSwitch;

// Container for the trigger status labels
@property (weak, nonatomic) IBOutlet UIView *switchStatusView;

// Labels used to indicate trigger status
@property (weak, nonatomic) IBOutlet UILabel *singlePressLabel;
@property (weak, nonatomic) IBOutlet UILabel *doublePressLabel;

@property (weak, nonatomic) IBOutlet UILabel *singlePressPolledLabel;
@property (weak, nonatomic) IBOutlet UILabel *doublePressPolledLabel;


// Enable/disable the switch state reporting
- (IBAction)reportsSwitchChanged;

// Enable/disable the polling
- (IBAction)pollingSwitchChanged;


@end
