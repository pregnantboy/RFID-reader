//
//  TSLDeviceFileLineReceived.h
//  TSLAsciiCommands
//
//  Created by Brian Painter on 07/10/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

///
/// The block type to handle device file information received as CRLF delimited lines
///
/// Commands will also invoke this block with line = nil and moreAvailable = NO
/// upon command completion
///
/// @param line - the line received from the device (without line end delimiters) or nil
/// @param moreAvailable - YES if there are more lines coming immediately
///
typedef void(^TSLFileLineReceivedBlock)(NSString *line, BOOL moreAvailable);

