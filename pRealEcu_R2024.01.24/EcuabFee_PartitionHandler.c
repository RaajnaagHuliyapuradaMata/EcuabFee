#include "Std_Types.hpp"

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_PartitionHandler.hpp"

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR (uint16, ECUABFEE_APPL_DATA) EcuabFee_Ph_PartitionIndex;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC (Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_Ph_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber);

ECUABFEE_LOCAL FUNC (uint16, ECUABFEE_PRIVATE_CODE) EcuabFee_Ph_CleanBlockNumber(uint16 BlockNumber);

ECUABFEE_LOCAL FUNC (Std_ReturnType, ECUABFEE_PRIVATE_CODE) EcuabFee_Ph_FindBlockNumberInList(uint16 PartitionIndex, uint16 BlockNumber)
{
  uint16 i;
  Std_ReturnType retVal = E_NOT_OK;

  for(i = 0; i < ECUABFEE_MAX_BLOCKS_PER_PARTITION; i++)
  {

   if(EcuabFee_PartitionConfigList[PartitionIndex].BlockNumberList[i] == BlockNumber)
   {
      retVal = E_OK;
      break;
   }
  }

  return retVal;
}

ECUABFEE_LOCAL FUNC (uint16, ECUABFEE_PRIVATE_CODE) EcuabFee_Ph_CleanBlockNumber(uint16 BlockNumber)
{
  return BlockNumber & (uint16) (0xFFFFU - ((1u << EcuabFee_DatasetSelectionBits) - 1u));
}

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_Init(uint16 BlockNumber)
{
  uint16 LoopCounter;

  for(LoopCounter = 0; LoopCounter < ECUABFEE_NUMBER_OF_PARTITIONS; LoopCounter++)
  {
   if(EcuabFee_Ph_FindBlockNumberInList(LoopCounter, EcuabFee_Ph_CleanBlockNumber(BlockNumber)) == E_OK)
   {

      EcuabFee_Ph_PartitionIndex = LoopCounter;
      break;
   }
  }
}

FUNC (EcuabFee_AddressType, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionStartAddress(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].PartitionStartAddress;
}

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionWriteAlignment(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].PartitionWriteAlignment;
}

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionAddressAlignment(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].PartitionAddressAlignment;
}

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetPartitionReadAlignment(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].PartitionReadAlignment;
}

FUNC (EcuabFee_FlsApiPtrType, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetFlsApiPtr(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].FlsApiPtr;
}

FUNC (uint8, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetFlsEraseValue(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].FlsEraseValue;
}

FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Ph_GetBlankCheckApi(void){
  return EcuabFee_PartitionConfigList[EcuabFee_Ph_PartitionIndex].BlankCheckApiEnabled;
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

