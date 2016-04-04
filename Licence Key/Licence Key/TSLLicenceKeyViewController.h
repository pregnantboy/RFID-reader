//
//  TSLViewController.h
//  Licence Key
//
//  Created by Brian Painter on 02/09/2014.
//  Copyright (c) 2014 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <TSLAsciiCommands/TSLAsciiCommands.h>

#import "TSLSelectReaderProtocol.h"


@interface TSLLicenceKeyViewController : UIViewController<UITextFieldDelegate, TSLInventoryCommandTransponderReceivedDelegate, TSLBarcodeCommandBarcodeReceivedDelegate, TSLSelectReaderProtocol>

@end
