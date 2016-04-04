//
//  TSLTransponderSelectCommand.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 02/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <TSLAsciiCommands/TSLAsciiCommands.h>

///
/// A command to set the state of transponders
///
/// Nte: This command is for use with persistent session targets only
///
@interface TSLTransponderSelectCommand : TSLAsciiSelfResponderCommandBase <TSLCommandParametersProtocol, TSLAntennaParametersProtocol, TSLSelectControlParametersProtocol, TSLSelectMaskParametersProtocol>

///
/// Returns a synchronous TSLTransponderSelectCommand
///
+(TSLTransponderSelectCommand *)synchronousCommand;


@end
