#ifndef EcuabFee_BLOCK_HANDLER_H
#define EcuabFee_BLOCK_HANDLER_H

#include "EcuabFee.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_Init(uint16 BlockNumber, EcuabFee_AddressType PartitionStartAddress);
FUNC (EcuabFee_AddressType, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetBlockStartAddress(void);
FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetDataLength(uint16 BlockNumber);
FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetNrOfInstances(uint16 BlockNumber);

#if(EcuabFee_DEV_ERROR_DETECT == STD_ON)
FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetNrOfDatasets(uint16 BlockNumber);
FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetDatasetIndex(uint16 BlockNumber);
FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_IsImmediateData(uint16 BlockNumber);
#endif

FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_HasVerificationEnabled(void);
FUNC (uint16, ECUABFEE_PRIVATE_CODE) EcuabFee_Bh_GetBlockIndex(uint16 BlockNumber);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

