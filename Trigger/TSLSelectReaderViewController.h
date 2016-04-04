//
//  TSLSelectReaderViewController.h
//  Inventory
//
//  Created by Brian Painter on 28/08/2014.
//  Copyright (c) 2014 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TSLSelectReaderProtocol.h"

@interface TSLSelectReaderViewController : UITableViewController

// The delegate to be informed of reader selections
@property (nonatomic, readwrite) id<TSLSelectReaderProtocol> delegate;

// Returns the identifier for the Select Reader segue
+(NSString *)segueIdentifier;


@end
