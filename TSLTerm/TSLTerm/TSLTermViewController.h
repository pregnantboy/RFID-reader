//
//  TSLViewController.h
//  TSLTerm
//
//  Created by Brian Painter on 22/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <TSLAsciiCommands/TSLAsciiCommands.h>

#import "TSLSelectReaderViewController.h"

@interface TSLTermViewController : UIViewController <UITableViewDelegate, UITableViewDataSource, UIPickerViewDelegate, TSLTerminalResponderDelegate, TSLSelectReaderProtocol>

@property (weak, nonatomic) IBOutlet UITextField *commandTextField;
@property (weak, nonatomic) IBOutlet UITableView *transcriptTableView;

- (IBAction)sendCommand;
- (IBAction)clearTranscript:(id)sender;

@end
