

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_BlockHandler.hpp"

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

typedef struct{
  uint16 BlockIndex;
  EcuabFee_AddressType AbsoluteBlockAddress;
}EcuabFee_Bh_BlockType;

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Bh_BlockType, ECUABFEE_APPL_DATA) EcuabFee_Bh_BlockInfo;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_Init(uint16 BlockNumber, EcuabFee_AddressType PartitionStartAddress){
  EcuabFee_Bh_BlockInfo.BlockIndex = EcuabFee_Bh_GetBlockIndex(BlockNumber);
  EcuabFee_Bh_BlockInfo.AbsoluteBlockAddress = PartitionStartAddress +
  EcuabFee_BlockConfigList[EcuabFee_Bh_BlockInfo.BlockIndex].BlockStartAddress;
}

FUNC (EcuabFee_AddressType, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetBlockStartAddress(void){
  return EcuabFee_Bh_BlockInfo.AbsoluteBlockAddress;
}

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetDataLength(uint16 BlockNumber){
  return EcuabFee_BlockConfigList[EcuabFee_Bh_GetBlockIndex(BlockNumber)].DataLength;
}

FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetNrOfInstances(uint16 BlockNumber){
  return EcuabFee_BlockConfigList[EcuabFee_Bh_GetBlockIndex(BlockNumber)].NumberOfInstances;
}

#if(ECUABFEE_DEV_ERROR_DETECT == STD_ON)

FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetNrOfDatasets(uint16 BlockNumber){
  return EcuabFee_BlockConfigList[EcuabFee_Bh_GetBlockIndex(BlockNumber)].NumberOfDatasets;
}

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_GetDatasetIndex(uint16 BlockNumber){
  return (uint16)(BlockNumber & ((1u << EcuabFee_DatasetSelectionBits) - 1u));
}

FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_IsImmediateData(uint16 BlockNumber){
  return EcuabFee_BlockConfigList[EcuabFee_Bh_GetBlockIndex(BlockNumber)].IsImmediateData;
}
#endif

FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Bh_HasVerificationEnabled(void){
  return EcuabFee_BlockConfigList[EcuabFee_Bh_BlockInfo.BlockIndex].HasVerificationEnabled;
}

FUNC (uint16, ECUABFEE_PRIVATE_CODE) EcuabFee_Bh_GetBlockIndex(uint16 BlockNumber){
  return (uint16)((uint16)(BlockNumber >> EcuabFee_DatasetSelectionBits) - 1u);
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

