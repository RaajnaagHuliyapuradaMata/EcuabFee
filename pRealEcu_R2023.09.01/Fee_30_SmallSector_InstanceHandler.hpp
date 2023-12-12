#ifndef EcuabFee_INSTANCE_HANDLER_H
#define EcuabFee_INSTANCE_HANDLER_H

#include "Fee_30_SmallSector.hpp"

typedef enum{
  INSTANCE_STATUS_ERASED = 0
   ,  INSTANCE_STATUS_OK
   ,  INSTANCE_STATUS_INVALID
   ,  INSTANCE_STATUS_INCONSISTENT
}EcuabFee_Ih_InstanceStatusType;

typedef P2VAR(uint8, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_Ih_BufferPointerType;

typedef struct{
  EcuabFee_AddressType EcuabFee_Ih_CompareAddress;
  uint16 EcuabFee_Ih_CompareLength;
  EcuabFee_Ih_BufferPointerType EcuabFee_Ih_CompareBufferPtr;
}EcuabFee_Ih_CompareParameterType;

typedef struct{
  EcuabFee_AddressType StartAddress;
  EcuabFee_AddressType WP2Address;
  EcuabFee_AddressType MgmtAddress;
  uint16 DataLength;
  EcuabFee_StaticPatternState WP1State;
  EcuabFee_StaticPatternState WP2State;
  EcuabFee_StaticPatternState LastPageState;
  uint8 MgmtValue;
  EcuabFee_Ih_InstanceStatusType InstanceStatus;
}EcuabFee_Ih_InstanceType;

typedef P2VAR(EcuabFee_Ih_CompareParameterType, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_Ih_CompareParameterPointerType;
typedef P2VAR(EcuabFee_Ih_InstanceType, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_Ih_InstanceVarPointerType;
typedef P2CONST(EcuabFee_Ih_InstanceType, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_Ih_InstanceConstPointerType;

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

FUNC(boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_IsLastPageNecessary(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_CreateInstance(EcuabFee_Ih_InstanceVarPointerType Instance
   ,       EcuabFee_AddressType InstanceStartAddress, uint16 DataLength);

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_InitInstance(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceStartAddress(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndPatternAddress(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetManagementAddress(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndAddress(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceDataLength(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_Ih_InstanceStatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceStatus(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC(uint8, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetMgmtValue(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetWP1State(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetWP2State(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetLastPageState(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_StoreMgmtValueFromBuffer(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetInstanceStatus(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_Ih_InstanceStatusType status);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetWP1State(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetWP2State(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetLastPageState(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_PrepareNextInstanceProperties(EcuabFee_Ih_InstanceVarPointerType Instance
   ,       EcuabFee_AddressType StartAddress);

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetManagementValuesAccordingToStatus(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC (MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetNegativeJobResultAccordingToStatus(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_DetermineInstanceStatus(EcuabFee_Ih_InstanceVarPointerType Instance);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_CorrectSingleBitFlips(EcuabFee_Ih_BufferPointerType ManagementByte);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetStartPatternFromBuffer(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndPatternFromBuffer(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetPageStatusFromBuffer(void);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckStartPattern(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckEndPattern(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckLastPage(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadLastPage(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadAllMgmt(EcuabFee_Ih_InstanceConstPointerType Instance);
FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadData(EcuabFee_Ih_InstanceConstPointerType Instance
   ,                                                   EcuabFee_Ih_BufferPointerType DataBufferPtr
   ,                                                   uint16 BlockOffset
   ,                                                   uint16 ReadLength);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteStartPattern(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_Ih_CompareParameterPointerType CompareParameter);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteManagementPage(EcuabFee_Ih_InstanceConstPointerType Instance
   ,       EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WritePayload(EcuabFee_Ih_InstanceConstPointerType Instance
   ,       EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteLastPage(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteEndPattern(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_Ih_CompareParameterPointerType CompareParameter);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_VerifyData(EcuabFee_Ih_CompareParameterType CompareParameter);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

