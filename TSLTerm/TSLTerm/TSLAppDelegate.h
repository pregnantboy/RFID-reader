//
//  TSLAppDelegate.h
//  TSLTerm
//
//  Created by Brian Painter on 22/05/2013.
//  Copyright (c) 2013 Technology Solutions (UK) Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <TSLAsciiCommands/TSLAsciiCommander.h>

@interface TSLAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (nonatomic, readonly) TSLAsciiCommander *commander;

@end
