#ifndef ECUABFEE_DATASET_HANDLER_H
#define ECUABFEE_DATASET_HANDLER_H

#include "EcuabFee.hpp"
#include "EcuabFee_InstanceHandler.hpp"

typedef enum{
  DATASET_ERASE_PATTERN_1 = 0
   ,  DATASET_ERASE_PATTERN_2
}EcuabFee_Dh_ErasePatternType;

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_Init(uint16 BlockNumber, uint16 DataLength
   ,     uint8 NumberOfInstances, EcuabFee_AddressType BlockStartAddress, uint16 AddressAlignment);

FUNC (uint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_GetDataLength(void);
FUNC (EcuabFee_AddressType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_GetFirstInstanceAddress(void);
FUNC (sint16, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_GetLastSectorIndex(void);
FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_PrepareNextInstance(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_EraseSector(uint16 sectorIndex);
FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_WriteErasePattern(EcuabFee_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_ReadErasePattern(EcuabFee_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_BlankCheckErasePattern(EcuabFee_Dh_ErasePatternType pattern);
FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_CheckErasePattern(void);
FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_IsLastInstance(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_IsFirstInstanceAndBlank(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_IsErasePatternBlank(void);
FUNC (boolean, ECUABFEE_PUBLIC_CODE) EcuabFee_Dh_IsErasePatternValid(void);
#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

