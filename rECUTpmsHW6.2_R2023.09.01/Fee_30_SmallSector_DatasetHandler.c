

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_PartitionHandler.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef struct{
  EcuabFee_AddressType DatasetStartAddress;
  EcuabFee_AddressType FirstInstanceAddress;
  EcuabFee_AddressType ErasePattern2Address;
  uint16 DatasetSize;
  uint16 AlignedInstanceSize;
  uint16 DataLength;
  uint8 NumberOfInstances;
  uint8 DatasetIndex;
}EcuabFee_Dh_DatasetType;

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Dh_DatasetType, EcuabFee_APPL_DATA) EcuabFee_Dh_DatasetInfo;

EcuabFee_LOCAL VAR(uint8, EcuabFee_APPL_DATA) EcuabFee_Dh_TempBuffer[EcuabFee_MAX_WRITE_ALIGNMENT];

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC (uint8, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetDatasetIndex(uint16 BlockNumber);

EcuabFee_LOCAL FUNC (uint8, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_CalcIndexOfCurrentInstance(EcuabFee_AddressType InstanceStartAddress);

EcuabFee_LOCAL FUNC (EcuabFee_AddressType, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance);

EcuabFee_LOCAL FUNC (uint16, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetAlignedSizeOfInstance(void);

EcuabFee_LOCAL FUNC (uint8, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetDatasetIndex(uint16 BlockNumber)
{
  return (uint8) (BlockNumber & ((1u << EcuabFee_DatasetSelectionBits) - 1u));
}

EcuabFee_LOCAL FUNC (uint8, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_CalcIndexOfCurrentInstance(EcuabFee_AddressType InstanceStartAddress)
{
  return (uint8) ((InstanceStartAddress - EcuabFee_Dh_DatasetInfo.DatasetStartAddress - EcuabFee_Ph_GetPartitionWriteAlignment()) / EcuabFee_Dh_GetAlignedSizeOfInstance());
}

EcuabFee_LOCAL FUNC (EcuabFee_AddressType, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetAddressFromInstanceIndex(uint8 IndexOfActiveInstance)
{

  return EcuabFee_Dh_DatasetInfo.DatasetStartAddress + (EcuabFee_AddressType)(EcuabFee_Ph_GetPartitionWriteAlignment() + (IndexOfActiveInstance * EcuabFee_Dh_GetAlignedSizeOfInstance()));
}

EcuabFee_LOCAL FUNC (uint16, EcuabFee_PRIVATE_CODE) EcuabFee_Dh_GetAlignedSizeOfInstance(void){
  return EcuabFee_Dh_DatasetInfo.AlignedInstanceSize;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_Init(uint16 BlockNumber, uint16 DataLength
   ,       uint8 NumberOfInstances, EcuabFee_AddressType BlockStartAddress, uint16 AddressAlignment)
{
  uint16 WriteAlignment = EcuabFee_Ph_GetPartitionWriteAlignment();

  EcuabFee_Dh_DatasetInfo.AlignedInstanceSize = (uint16) EcuabFee_AlignValue((2u * WriteAlignment) + EcuabFee_MANAGEMENT_SIZE + DataLength, WriteAlignment);

  EcuabFee_Dh_DatasetInfo.DatasetSize = (uint16) EcuabFee_AlignValue((uint32) ((2u * WriteAlignment) + (EcuabFee_Dh_DatasetInfo.AlignedInstanceSize * NumberOfInstances)), AddressAlignment);

  EcuabFee_Dh_DatasetInfo.DatasetIndex = EcuabFee_Dh_GetDatasetIndex(BlockNumber);

  EcuabFee_Dh_DatasetInfo.DatasetStartAddress = BlockStartAddress + (EcuabFee_AddressType)(EcuabFee_Dh_DatasetInfo.DatasetSize * EcuabFee_Dh_DatasetInfo.DatasetIndex);

  EcuabFee_Dh_DatasetInfo.FirstInstanceAddress = EcuabFee_Dh_DatasetInfo.DatasetStartAddress + WriteAlignment;

  EcuabFee_Dh_DatasetInfo.ErasePattern2Address = (EcuabFee_Dh_DatasetInfo.DatasetStartAddress + EcuabFee_Dh_DatasetInfo.DatasetSize) - WriteAlignment;
  EcuabFee_Dh_DatasetInfo.DataLength = DataLength;
  EcuabFee_Dh_DatasetInfo.NumberOfInstances = NumberOfInstances;
}

FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_GetDataLength(void){
  return EcuabFee_Dh_DatasetInfo.DataLength;
}

FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_GetFirstInstanceAddress(void){
  return EcuabFee_Dh_DatasetInfo.FirstInstanceAddress;
}

FUNC (sint16, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_GetLastSectorIndex(void){

  return (sint16)((EcuabFee_Dh_DatasetInfo.DatasetSize / EcuabFee_Ph_GetPartitionAddressAlignment()) - 1u);
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_PrepareNextInstance(EcuabFee_Ih_InstanceVarPointerType Instance)
{

  uint8 nextInstanceIndex;
  EcuabFee_AddressType NewStartAddress;

  nextInstanceIndex = (uint8) (1u + EcuabFee_Dh_CalcIndexOfCurrentInstance(EcuabFee_Ih_GetInstanceStartAddress(Instance)));

  NewStartAddress = EcuabFee_Dh_GetAddressFromInstanceIndex(nextInstanceIndex);

  EcuabFee_Ih_PrepareNextInstanceProperties(Instance, NewStartAddress);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_EraseSector(uint16 sectorIndex)
{

  EcuabFee_AddressType sectorAddress = EcuabFee_Dh_DatasetInfo.DatasetStartAddress + (EcuabFee_AddressType) (EcuabFee_Ph_GetPartitionAddressAlignment() * sectorIndex);

  return EcuabFee_Fls_Erase(sectorAddress, EcuabFee_Ph_GetPartitionAddressAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_WriteErasePattern(EcuabFee_Dh_ErasePatternType pattern)
{

  uint8 LoopCounter;

  EcuabFee_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? EcuabFee_Dh_DatasetInfo.DatasetStartAddress : EcuabFee_Dh_DatasetInfo.ErasePattern2Address);

  for(LoopCounter = 0u; LoopCounter < EcuabFee_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    EcuabFee_Dh_TempBuffer[LoopCounter] = EcuabFee_STATIC_PATTERN;
  }

  return EcuabFee_Fls_Write(address, EcuabFee_Dh_TempBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_ReadErasePattern(EcuabFee_Dh_ErasePatternType pattern)
{

  EcuabFee_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? EcuabFee_Dh_DatasetInfo.DatasetStartAddress : EcuabFee_Dh_DatasetInfo.ErasePattern2Address);

  return EcuabFee_Fls_Read(address, EcuabFee_Dh_TempBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_BlankCheckErasePattern(EcuabFee_Dh_ErasePatternType pattern)
{

  EcuabFee_AddressType address = ((pattern == DATASET_ERASE_PATTERN_1) ? EcuabFee_Dh_DatasetInfo.DatasetStartAddress : EcuabFee_Dh_DatasetInfo.ErasePattern2Address);

  return EcuabFee_Fls_BlankCheck(address, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_CheckErasePattern(void){

  Std_ReturnType retVal;

  if(EcuabFee_Ph_GetBlankCheckApi() == TRUE)
  {

    retVal = EcuabFee_Dh_BlankCheckErasePattern(DATASET_ERASE_PATTERN_1);
  }
  else{

    retVal = EcuabFee_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_1);
  }
  return retVal;
}

FUNC (boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_IsLastInstance(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  uint8 IndexOfInstance = EcuabFee_Dh_CalcIndexOfCurrentInstance(EcuabFee_Ih_GetInstanceStartAddress(Instance));

  return ((IndexOfInstance == (EcuabFee_Dh_DatasetInfo.NumberOfInstances - 1u)) ? TRUE : FALSE);
}

FUNC (boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_IsFirstInstanceAndBlank(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  uint8 IndexOfInstance = EcuabFee_Dh_CalcIndexOfCurrentInstance(EcuabFee_Ih_GetInstanceStartAddress(Instance));

  boolean IsBlank = (boolean) (EcuabFee_Ih_GetInstanceStatus(Instance) == INSTANCE_STATUS_ERASED);

  return (((IndexOfInstance == 0) && (IsBlank == TRUE)) ? TRUE : FALSE);
}

FUNC (boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_IsErasePatternBlank(void){
  boolean retVal = TRUE;
  uint16 LoopCounter;

  if(EcuabFee_Ph_GetBlankCheckApi() == FALSE)
  {

   for(LoopCounter = 0; LoopCounter < EcuabFee_Ph_GetPartitionWriteAlignment(); LoopCounter++)
   {
      if(EcuabFee_Dh_TempBuffer[LoopCounter] != EcuabFee_Ph_GetFlsEraseValue())
      {

        retVal = FALSE;
        break;
      }
   }
  }

  return retVal;
}

FUNC (boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Dh_IsErasePatternValid(void){
  boolean retVal = TRUE;
  uint16 LoopCounter;

  for(LoopCounter = 0; LoopCounter < EcuabFee_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {
   if(EcuabFee_Dh_TempBuffer[LoopCounter] != EcuabFee_STATIC_PATTERN)
   {

      retVal = FALSE;
      break;
   }
  }

  return retVal;
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

