//
//  TSLScrollingTranscriptCell.h
//  TSL UHF RFID Explorer
//
//  Created by Scrith on 05/12/2012.
//  Copyright (c) 2012 Scrith. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TSLScrollingTranscriptCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UILabel *label;

@end
