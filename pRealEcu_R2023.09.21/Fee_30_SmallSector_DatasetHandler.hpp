#ifndef FEE_30_SMALLSECTOR_DATASET_HANDLER_H
#define FEE_30_SMALLSECTOR_DATASET_HANDLER_H

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_InstanceHandler.hpp"

typedef enum{
  DATASET_ERASE_PATTERN_1 = 0
   ,  DATASET_ERASE_PATTERN_2
}Fee_30_SmallSector_Dh_ErasePatternType;

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_Init(uint16 BlockNumber, uint16 DataLength
   ,     uint8 NumberOfInstances, Fee_30_SmallSector_AddressType BlockStartAddress, uint16 AddressAlignment);

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetDataLength(void);
FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetFirstInstanceAddress(void);
FUNC (sint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_GetLastSectorIndex(void);
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_PrepareNextInstance(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance);
FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_EraseSector(uint16 sectorIndex);
FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_WriteErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_ReadErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_BlankCheckErasePattern(Fee_30_SmallSector_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_CheckErasePattern(void);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsLastInstance(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsFirstInstanceAndBlank(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsErasePatternBlank(void);
FUNC (boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Dh_IsErasePatternValid(void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

