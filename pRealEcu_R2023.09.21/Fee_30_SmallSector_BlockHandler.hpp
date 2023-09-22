#ifndef FEE_30_SMALLSECTOR_BLOCK_HANDLER_H
#define FEE_30_SMALLSECTOR_BLOCK_HANDLER_H

#include "Fee_30_SmallSector.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_Init(uint16 BlockNumber, Fee_30_SmallSector_AddressType PartitionStartAddress);
FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetBlockStartAddress(void);
FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetDataLength(uint16 BlockNumber);
FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetNrOfInstances(uint16 BlockNumber);

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)
FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetNrOfDatasets(uint16 BlockNumber);
FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetDatasetIndex(uint16 BlockNumber);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_IsImmediateData(uint16 BlockNumber);
#endif

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_HasVerificationEnabled(void);
FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Bh_GetBlockIndex(uint16 BlockNumber);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

