//
//  TSLSelectMaskParameters.h
//  AgoutiCommands
//
//  Created by Brian Painter on 04/04/2013.
//  Copyright (c) 2013 Technology Solutions UK Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "TSLTriState.h"
#import "TSLParametersProtocol.h"
#import "TSLDataBankParameters.h"

///
/// Parameters for Select Mask operations in commands and responses
///
@protocol TSLSelectMaskParametersProtocol <NSObject>

/// @name Specifying the Select mask

/// Bank to use for the select mask
@property (nonatomic, readwrite) TSL_DataBank selectBank;

/// Select mask data in 2 character ASCII Hex pairs padded to ensure full bytes
@property (nonatomic, readwrite) NSString *selectData;

/// Number of bits from the start of the block to the start of the select mask
@property (nonatomic, readwrite) int selectOffset;

/// Length in bits of the select mask
@property (nonatomic, readwrite) int selectLength;

@end


///
/// Helper class for implementing TSLSelectMaskParametersProtocol
///
@interface TSLSelectMaskParameters : NSObject <TSLParametersProtocol>


@end
