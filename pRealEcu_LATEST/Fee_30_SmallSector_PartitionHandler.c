#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_PartitionHandler.hpp"

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR (uint16, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Ph_PartitionIndex;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ph_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber);

FEE_30_SMALLSECTOR_LOCAL FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ph_CleanBlockNumber(uint16 BlockNumber);

FEE_30_SMALLSECTOR_LOCAL FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ph_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber)
{
  uint16 i;
  Std_ReturnType retVal = E_NOT_OK;

  for(i = 0; i < FEE_30_SMALLSECTOR_MAX_BLOCKS_PER_PARTITION; i++)
  {

   if(Fee_30_SmallSector_PartitionConfigList[PartitionIndex].BlockNumberList[i] == BlockNumber)
   {
      retVal = E_OK;
      break;
   }
  }

  return retVal;
}

FEE_30_SMALLSECTOR_LOCAL FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ph_CleanBlockNumber(uint16 BlockNumber)
{
  return BlockNumber & (uint16) (0xFFFFU - ((1u << Fee_30_SmallSector_DatasetSelectionBits) - 1u));
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_Init(uint16 BlockNumber)
{
  uint16 LoopCounter;

  for(LoopCounter = 0; LoopCounter < FEE_30_SMALLSECTOR_NUMBER_OF_PARTITIONS; LoopCounter++)
  {
   if(Fee_30_SmallSector_Ph_FindBlockNumberInList(LoopCounter, Fee_30_SmallSector_Ph_CleanBlockNumber(BlockNumber)) == E_OK)
   {

      Fee_30_SmallSector_Ph_PartitionIndex = LoopCounter;
      break;
   }
  }
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionStartAddress(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].PartitionStartAddress;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].PartitionWriteAlignment;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionAddressAlignment(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].PartitionAddressAlignment;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetPartitionReadAlignment(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].PartitionReadAlignment;
}

FUNC (Fee_30_SmallSector_FlsApiPtrType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetFlsApiPtr(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].FlsApiPtr;
}

FUNC (uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetFlsEraseValue(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].FlsEraseValue;
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ph_GetBlankCheckApi(void){
  return Fee_30_SmallSector_PartitionConfigList[Fee_30_SmallSector_Ph_PartitionIndex].BlankCheckApiEnabled;
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

