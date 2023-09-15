

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_DatasetHandler.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef struct{
  Fee_30_SmallSector_AddressType DatasetStartAddress;
  Fee_30_SmallSector_AddressType FirstInstanceAddress;
  Fee_30_SmallSector_AddressType ErasePattern2Address;
  uint16 DatasetSize;
  uint16 AlignedInstanceSize;
  uint16 DataLength;
  uint8 NumberOfInstances;
  uint8 DatasetIndex;
}Fee_30_SmallSector_Dh_DatasetType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Dh_DatasetType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Dh_DatasetInfo;

FEE_30_SMALLSECTOR_LOCAL VAR(uint8, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Dh_TempBuffer[FEE_30_SMALLSECTOR_MAX_WRITE_ALIGNMENT];

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC (uint8, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetDatasetIndex(uint16 BlockNumber);

FEE_30_SMALLSECTOR_LOCAL FUNC (uint8, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_CalcIndexOfCurrentInstance(Fee_30_SmallSector_AddressType InstanceStartAddress);

FEE_30_SMALLSECTOR_LOCAL FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance);

FEE_30_SMALLSECTOR_LOCAL FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetAlignedSizeOfInstance(void);

FEE_30_SMALLSECTOR_LOCAL FUNC (uint8, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetDatasetIndex(uint16 BlockNumber)
{
  return (uint8) (BlockNumber & ((1u << Fee_30_SmallSector_DatasetSelectionBits) - 1u));
}

FEE_30_SMALLSECTOR_LOCAL FUNC (uint8, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_CalcIndexOfCurrentInstance(Fee_30_SmallSector_AddressType InstanceStartAddress)
{
  return (uint8) ((InstanceStartAddress - Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress - Fee_30_SmallSector_Ph_GetPartitionWriteAlignment()) / Fee_30_SmallSector_Dh_GetAlignedSizeOfInstance());
}

FEE_30_SMALLSECTOR_LOCAL FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance)
{

  return Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress + (Fee_30_SmallSector_AddressType)(Fee_30_SmallSector_Ph_GetPartitionWriteAlignment() + (IndexOfActiveInstance * Fee_30_SmallSector_Dh_GetAlignedSizeOfInstance()));
}

FEE_30_SMALLSECTOR_LOCAL FUNC (uint16, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Dh_GetAlignedSizeOfInstance(void){
  return Fee_30_SmallSector_Dh_DatasetInfo.AlignedInstanceSize;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_Init(uint16 BlockNumber, uint16 DataLength
   ,       uint8 NumberOfInstances, Fee_30_SmallSector_AddressType BlockStartAddress, uint16 AddressAlignment)
{
  uint16 WriteAlignment = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  Fee_30_SmallSector_Dh_DatasetInfo.AlignedInstanceSize = (uint16) Fee_30_SmallSector_AlignValue((2u * WriteAlignment) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE + DataLength, WriteAlignment);

  Fee_30_SmallSector_Dh_DatasetInfo.DatasetSize = (uint16) Fee_30_SmallSector_AlignValue((uint32) ((2u * WriteAlignment) + (Fee_30_SmallSector_Dh_DatasetInfo.AlignedInstanceSize * NumberOfInstances)), AddressAlignment);

  Fee_30_SmallSector_Dh_DatasetInfo.DatasetIndex = Fee_30_SmallSector_Dh_GetDatasetIndex(BlockNumber);

  Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress = BlockStartAddress + (Fee_30_SmallSector_AddressType)(Fee_30_SmallSector_Dh_DatasetInfo.DatasetSize * Fee_30_SmallSector_Dh_DatasetInfo.DatasetIndex);

  Fee_30_SmallSector_Dh_DatasetInfo.FirstInstanceAddress = Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress + WriteAlignment;

  Fee_30_SmallSector_Dh_DatasetInfo.ErasePattern2Address = (Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress + Fee_30_SmallSector_Dh_DatasetInfo.DatasetSize) - WriteAlignment;
  Fee_30_SmallSector_Dh_DatasetInfo.DataLength = DataLength;
  Fee_30_SmallSector_Dh_DatasetInfo.NumberOfInstances = NumberOfInstances;
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetDataLength(void){
  return Fee_30_SmallSector_Dh_DatasetInfo.DataLength;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetFirstInstanceAddress(void){
  return Fee_30_SmallSector_Dh_DatasetInfo.FirstInstanceAddress;
}

FUNC (sint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetLastSectorIndex(void){

  return (sint16)((Fee_30_SmallSector_Dh_DatasetInfo.DatasetSize / Fee_30_SmallSector_Ph_GetPartitionAddressAlignment()) - 1u);
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_PrepareNextInstance(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{

  uint8 nextInstanceIndex;
  Fee_30_SmallSector_AddressType NewStartAddress;

  nextInstanceIndex = (uint8) (1u + Fee_30_SmallSector_Dh_CalcIndexOfCurrentInstance(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance)));

  NewStartAddress = Fee_30_SmallSector_Dh_GetAddressFromInstanceIndex(nextInstanceIndex);

  Fee_30_SmallSector_Ih_PrepareNextInstanceProperties(Instance, NewStartAddress);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_EraseSector(uint16 sectorIndex)
{

  Fee_30_SmallSector_AddressType sectorAddress = Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress + (Fee_30_SmallSector_AddressType) (Fee_30_SmallSector_Ph_GetPartitionAddressAlignment() * sectorIndex);

  return Fee_30_SmallSector_Fls_Erase(sectorAddress, Fee_30_SmallSector_Ph_GetPartitionAddressAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_WriteErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern)
{

  uint8 LoopCounter;

  Fee_30_SmallSector_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress : Fee_30_SmallSector_Dh_DatasetInfo.ErasePattern2Address);

  for(LoopCounter = 0u; LoopCounter < FEE_30_SMALLSECTOR_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    Fee_30_SmallSector_Dh_TempBuffer[LoopCounter] = FEE_30_SMALLSECTOR_STATIC_PATTERN;
  }

  return Fee_30_SmallSector_Fls_Write(address, Fee_30_SmallSector_Dh_TempBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_ReadErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern)
{

  Fee_30_SmallSector_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress : Fee_30_SmallSector_Dh_DatasetInfo.ErasePattern2Address);

  return Fee_30_SmallSector_Fls_Read(address, Fee_30_SmallSector_Dh_TempBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_BlankCheckErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern)
{

  Fee_30_SmallSector_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? Fee_30_SmallSector_Dh_DatasetInfo.DatasetStartAddress : Fee_30_SmallSector_Dh_DatasetInfo.ErasePattern2Address);

  return Fee_30_SmallSector_Fls_BlankCheck(address, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_CheckErasePattern(void){

  Std_ReturnType retVal;

  if(Fee_30_SmallSector_Ph_GetBlankCheckApi() == TRUE)
  {

    retVal = Fee_30_SmallSector_Dh_BlankCheckErasePattern(DATASET_ERASE_PATTERN_1);
  }
  else{

    retVal = Fee_30_SmallSector_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_1);
  }
  return retVal;
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsLastInstance(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  uint8 IndexOfInstance = Fee_30_SmallSector_Dh_CalcIndexOfCurrentInstance(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance));

  return ((IndexOfInstance == (Fee_30_SmallSector_Dh_DatasetInfo.NumberOfInstances - 1u)) ? TRUE : FALSE);
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsFirstInstanceAndBlank(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  uint8 IndexOfInstance = Fee_30_SmallSector_Dh_CalcIndexOfCurrentInstance(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance));

  boolean IsBlank = (boolean) (Fee_30_SmallSector_Ih_GetInstanceStatus(Instance) == INSTANCE_STATUS_ERASED);

  return (((IndexOfInstance == 0) && (IsBlank == TRUE)) ? TRUE : FALSE);
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsErasePatternBlank(void){
  boolean retVal = TRUE;
  uint16 LoopCounter;

  if(Fee_30_SmallSector_Ph_GetBlankCheckApi() == FALSE)
  {

   for(LoopCounter = 0; LoopCounter < Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(); LoopCounter++)
   {
      if(Fee_30_SmallSector_Dh_TempBuffer[LoopCounter] != Fee_30_SmallSector_Ph_GetFlsEraseValue())
      {

        retVal = FALSE;
        break;
      }
   }
  }

  return retVal;
}

FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsErasePatternValid(void){
  boolean retVal = TRUE;
  uint16 LoopCounter;

  for(LoopCounter = 0; LoopCounter < Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {
   if(Fee_30_SmallSector_Dh_TempBuffer[LoopCounter] != FEE_30_SMALLSECTOR_STATIC_PATTERN)
   {

      retVal = FALSE;
      break;
   }
  }

  return retVal;
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

