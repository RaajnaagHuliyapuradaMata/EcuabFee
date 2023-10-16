#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_BlockHandler.hpp"

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef struct{
  uint16 BlockIndex;
  Fee_30_SmallSector_AddressType AbsoluteBlockAddress;
}Fee_30_SmallSector_Bh_BlockType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Bh_BlockType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Bh_BlockInfo;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_Init(uint16 BlockNumber, Fee_30_SmallSector_AddressType PartitionStartAddress)
{

  Fee_30_SmallSector_Bh_BlockInfo.BlockIndex = Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber);
  Fee_30_SmallSector_Bh_BlockInfo.AbsoluteBlockAddress = PartitionStartAddress +
  Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_BlockInfo.BlockIndex].BlockStartAddress;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetBlockStartAddress(void){
  return Fee_30_SmallSector_Bh_BlockInfo.AbsoluteBlockAddress;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetDataLength(uint16 BlockNumber)
{
  return Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber)].DataLength;
}

FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetNrOfInstances(uint16 BlockNumber)
{
  return Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber)].NumberOfInstances;
}

#if(FEE_30_SMALLSECTOR_DEV_ERROR_DETECT == STD_ON)

FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetNrOfDatasets(uint16 BlockNumber)
{
  return Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber)].NumberOfDatasets;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_GetDatasetIndex(uint16 BlockNumber)
{

  return (uint16)(BlockNumber & ((1u << Fee_30_SmallSector_DatasetSelectionBits) - 1u));
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_IsImmediateData(uint16 BlockNumber)
{
  return Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_GetBlockIndex(BlockNumber)].IsImmediateData;
}
#endif

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Bh_HasVerificationEnabled(void){
  return Fee_30_SmallSector_BlockConfigList[Fee_30_SmallSector_Bh_BlockInfo.BlockIndex].HasVerificationEnabled;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Bh_GetBlockIndex(uint16 BlockNumber)
{
  return (uint16)((uint16)(BlockNumber >> Fee_30_SmallSector_DatasetSelectionBits) - 1u);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

