

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_InstanceHandler.hpp"
#include "EcuabFee_PartitionHandler.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#define EcuabFee_CYCLE_COUNTER_BIT_MASK  (0x3U)

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(uint8, EcuabFee_APPL_DATA) EcuabFee_Ih_ReadBuffer[EcuabFee_MAX_READ_SIZE];
EcuabFee_LOCAL VAR(uint8, EcuabFee_APPL_DATA) EcuabFee_Ih_WriteBuffer[EcuabFee_MAX_WRITE_ALIGNMENT];

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CONST_8BIT
#include "MemMap.hpp"

EcuabFee_LOCAL CONST(uint8, EcuabFee_APPL_DATA) EcuabFee_Ih_ValidManagementValue = 0xF0u;
EcuabFee_LOCAL CONST(uint8, EcuabFee_APPL_DATA) EcuabFee_Ih_InvalidManagementValue = 0x0Fu;

#define EcuabFee_STOP_SEC_CONST_8BIT
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_IsValidManagement(uint8 ManagementByte);

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_IsInvalidManagement(uint8 ManagementByte);

#if(EcuabFee_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_HasEvenParity(uint8 ManagementNibble);
#endif

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_FillBufferWithStaticPattern(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_FillBufferWithErasedValue(void);

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_IsValidManagement(uint8 ManagementByte)
{
  return (ManagementByte == EcuabFee_Ih_ValidManagementValue) ? TRUE : FALSE;
}

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_IsInvalidManagement(uint8 ManagementByte)
{
  return (ManagementByte == EcuabFee_Ih_InvalidManagementValue) ? TRUE : FALSE;
}

#if(EcuabFee_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

EcuabFee_LOCAL FUNC(boolean, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_HasEvenParity(uint8 ManagementNibble)
{
  boolean retVal;

  ManagementNibble = ManagementNibble & 0x0Fu;

  ManagementNibble ^= (uint8) (ManagementNibble >> 2u);

  ManagementNibble ^= (uint8) (ManagementNibble >> 1u);

  if((ManagementNibble & 0x01u) == 0x00u)
  {
    retVal = TRUE;
  }
  else{
    retVal = FALSE;
  }

  return retVal;
}
#endif

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_FillBufferWithStaticPattern(void){
  uint8 LoopCounter;

  for(LoopCounter = 0u; LoopCounter < EcuabFee_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    EcuabFee_Ih_WriteBuffer[LoopCounter] = EcuabFee_STATIC_PATTERN;
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Ih_FillBufferWithErasedValue(void){
  uint8 LoopCounter;

  for(LoopCounter = 0u; LoopCounter < EcuabFee_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    EcuabFee_Ih_WriteBuffer[LoopCounter] = EcuabFee_Ph_GetFlsEraseValue();
  }
}

FUNC(boolean, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_IsLastPageNecessary(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  return (boolean) ((EcuabFee_Ih_GetInstanceDataLength(Instance) + EcuabFee_MANAGEMENT_SIZE) % EcuabFee_Ph_GetPartitionWriteAlignment()) != 0;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_CreateInstance(EcuabFee_Ih_InstanceVarPointerType Instance
   ,       EcuabFee_AddressType InstanceStartAddress, uint16 DataLength)
{
  Instance->StartAddress = InstanceStartAddress;
  Instance->WP2Address = InstanceStartAddress + EcuabFee_Ph_GetPartitionWriteAlignment();
  Instance->MgmtAddress = InstanceStartAddress + (EcuabFee_AddressType) (2 * EcuabFee_Ph_GetPartitionWriteAlignment());
  Instance->DataLength = DataLength;
  Instance->MgmtValue = EcuabFee_Ph_GetFlsEraseValue();
  Instance->WP1State = EcuabFee_PATTERN_INIT;
  Instance->WP2State = EcuabFee_PATTERN_INIT;
  Instance->LastPageState = EcuabFee_PATTERN_INIT;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_InitInstance(EcuabFee_Ih_InstanceVarPointerType Instance)
{
  Instance->StartAddress = 0x0000U;
  Instance->WP2Address = 0x0000U;
  Instance->MgmtAddress = 0x0000U;
  Instance->DataLength = 0x0000U;
  Instance->WP1State = EcuabFee_PATTERN_INIT;
  Instance->WP2State = EcuabFee_PATTERN_INIT;
  Instance->MgmtValue = 0x00U;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceStartAddress(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->StartAddress;
}

FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndPatternAddress(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP2Address;
}

FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetManagementAddress(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->MgmtAddress;
}

FUNC (EcuabFee_AddressType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndAddress(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  return EcuabFee_AlignValue(EcuabFee_Ih_GetManagementAddress(Instance) + EcuabFee_MANAGEMENT_SIZE + EcuabFee_Ih_GetInstanceDataLength(Instance)
   ,     EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (uint16, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceDataLength(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->DataLength;
}

FUNC (EcuabFee_Ih_InstanceStatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetInstanceStatus(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->InstanceStatus;
}

FUNC(uint8, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetMgmtValue(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->MgmtValue;
}

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetWP1State(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP1State;
}

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetWP2State(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP2State;
}

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetLastPageState(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return Instance->LastPageState;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_StoreMgmtValueFromBuffer(EcuabFee_Ih_InstanceVarPointerType Instance)
{

  Instance->MgmtValue = EcuabFee_Ih_ReadBuffer[2 * EcuabFee_Ph_GetPartitionWriteAlignment()];
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetInstanceStatus(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_Ih_InstanceStatusType status)
{
  Instance->InstanceStatus = status;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetWP1State(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state)
{
  Instance->WP1State = state;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetWP2State(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state)
{
  Instance->WP2State = state;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetLastPageState(EcuabFee_Ih_InstanceVarPointerType Instance, EcuabFee_StaticPatternState state)
{
  Instance->LastPageState = state;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_PrepareNextInstanceProperties(EcuabFee_Ih_InstanceVarPointerType Instance
   ,       EcuabFee_AddressType StartAddress)
{

  Instance->StartAddress = StartAddress;
  Instance->WP2Address = StartAddress + EcuabFee_Ph_GetPartitionWriteAlignment();
  Instance->MgmtAddress = StartAddress + (EcuabFee_AddressType) (2 * EcuabFee_Ph_GetPartitionWriteAlignment());

  Instance->MgmtValue = EcuabFee_Ph_GetFlsEraseValue();
  Instance->WP1State = EcuabFee_PATTERN_INIT;
  Instance->WP2State = EcuabFee_PATTERN_INIT;
  Instance->LastPageState = EcuabFee_PATTERN_INIT;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetManagementValuesAccordingToStatus(EcuabFee_Ih_InstanceVarPointerType Instance)
{

  if(EcuabFee_Ih_GetInstanceStatus(Instance) == INSTANCE_STATUS_OK)
  {

    Instance->MgmtValue = EcuabFee_Ih_ValidManagementValue;
  }
  else{

    Instance->MgmtValue = EcuabFee_Ih_InvalidManagementValue;
  }
}

FUNC (MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_SetNegativeJobResultAccordingToStatus(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  MemIf_JobResultType retVal;

  switch(EcuabFee_Ih_GetInstanceStatus(Instance))
  {
   case INSTANCE_STATUS_INVALID:
   case INSTANCE_STATUS_ERASED:
      retVal = MEMIF_BLOCK_INVALID;
      break;
    default:
      retVal = MEMIF_BLOCK_INCONSISTENT;
      break;
  }

  return retVal;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_DetermineInstanceStatus(EcuabFee_Ih_InstanceVarPointerType Instance)
{
  EcuabFee_Ih_InstanceStatusType status;
  uint8 mgmtValue = EcuabFee_Ih_GetMgmtValue(Instance);
  EcuabFee_StaticPatternState wp1State = EcuabFee_Ih_GetWP1State(Instance);
  EcuabFee_StaticPatternState wp2State = EcuabFee_Ih_GetWP2State(Instance);
  EcuabFee_StaticPatternState lastPageState = EcuabFee_Ih_GetLastPageState(Instance);

  if((wp1State == EcuabFee_PATTERN_BLANK) &&
      (wp2State == EcuabFee_PATTERN_BLANK) &&
      (lastPageState == EcuabFee_PATTERN_BLANK)
  )
  {
   status = INSTANCE_STATUS_ERASED;
  }

  else if( (wp1State == EcuabFee_PATTERN_VALID) &&
            (wp2State == EcuabFee_PATTERN_VALID) )
  {

   if((EcuabFee_Ih_IsValidManagement(mgmtValue) == TRUE) &&
        (lastPageState != EcuabFee_PATTERN_BLANK))
   {
      status = INSTANCE_STATUS_OK;
   }

   else if(EcuabFee_Ih_IsInvalidManagement(mgmtValue) == TRUE)
   {
      status = INSTANCE_STATUS_INVALID;
   }

   else{
      status = INSTANCE_STATUS_INCONSISTENT;
   }

  }

  else{
   status = INSTANCE_STATUS_INCONSISTENT;
  }

  Instance->InstanceStatus = status;
}

#if(EcuabFee_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_CorrectSingleBitFlips(EcuabFee_Ih_BufferPointerType ManagementByte)
{
  uint8 correctNibble;
  uint8 LowerNibble = (*ManagementByte) & 0x0Fu;
  uint8 HigherNibbleInverted = ((   (uint8)( ~(*ManagementByte) )   ) >> 4u) & 0x0Fu;
  uint8 XorLowerHigherNibble = LowerNibble ^ HigherNibbleInverted;

  if(LowerNibble != HigherNibbleInverted)
  {

   if((XorLowerHigherNibble & (XorLowerHigherNibble - 1u)) == 0u)
   {

      if(EcuabFee_Ih_HasEvenParity(LowerNibble) == TRUE)
      {
        correctNibble = LowerNibble;
      }
      else{
        correctNibble = HigherNibbleInverted;
      }

      if((correctNibble & 4u) == 0u)
      {

        *ManagementByte = EcuabFee_Ih_ValidManagementValue;
      }
      else{

        *ManagementByte = EcuabFee_Ih_InvalidManagementValue;
      }
   }
  }
}
#endif

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetStartPatternFromBuffer(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  uint16 LoopCounter;
  EcuabFee_StaticPatternState retVal = EcuabFee_PATTERN_INIT;
  boolean isPatternValid = FALSE;
  boolean isPatternBlank = FALSE;

  for(LoopCounter = 0; LoopCounter < EcuabFee_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {

   if(EcuabFee_Ih_ReadBuffer[LoopCounter] == EcuabFee_STATIC_PATTERN)
   {
      isPatternValid = TRUE;
      retVal = EcuabFee_PATTERN_VALID;
   }

   else if(EcuabFee_Ih_ReadBuffer[LoopCounter] == EcuabFee_Ph_GetFlsEraseValue())
   {
      isPatternBlank = TRUE;
      retVal = EcuabFee_PATTERN_BLANK;
   }
   else{

      isPatternBlank = TRUE;
      isPatternValid = TRUE;
      break;
   }
  }

  if((isPatternValid ^ isPatternBlank) == FALSE)
  {
    retVal = EcuabFee_PATTERN_INCONSISTENT;
  }

  if(EcuabFee_Ih_GetWP1State(Instance) == EcuabFee_PATTERN_INCONSISTENT)
  {
   if(retVal == EcuabFee_PATTERN_BLANK)
   {

      retVal = EcuabFee_PATTERN_INCONSISTENT;
   }

  }

  return retVal;
}

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetEndPatternFromBuffer(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  uint16 LoopCounter;
  EcuabFee_StaticPatternState retVal = EcuabFee_PATTERN_INIT;
  boolean isPatternValid = FALSE;
  boolean isPatternBlank = FALSE;

  for(LoopCounter = EcuabFee_Ph_GetPartitionWriteAlignment(); LoopCounter < (2 * EcuabFee_Ph_GetPartitionWriteAlignment()); LoopCounter++)
  {

   if(EcuabFee_Ih_ReadBuffer[LoopCounter] == EcuabFee_STATIC_PATTERN)
   {
      isPatternValid = TRUE;
      retVal = EcuabFee_PATTERN_VALID;
   }

   else if(EcuabFee_Ih_ReadBuffer[LoopCounter] == EcuabFee_Ph_GetFlsEraseValue())
   {
      isPatternBlank = TRUE;
      retVal = EcuabFee_PATTERN_BLANK;
   }
   else{

      isPatternBlank = TRUE;
      isPatternValid = TRUE;
      break;
   }
  }

  if((isPatternValid ^ isPatternBlank) == FALSE)
  {
    retVal = EcuabFee_PATTERN_INCONSISTENT;
  }

  if(EcuabFee_Ih_GetWP2State(Instance) == EcuabFee_PATTERN_INCONSISTENT)
  {
   if(retVal == EcuabFee_PATTERN_BLANK)
   {

      retVal = EcuabFee_PATTERN_INCONSISTENT;
   }

  }

  return retVal;
}

FUNC (EcuabFee_StaticPatternState, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_GetPageStatusFromBuffer(void){
  uint16 LoopCounter;
  EcuabFee_StaticPatternState retVal = EcuabFee_PATTERN_BLANK;

  for(LoopCounter = 0; LoopCounter < EcuabFee_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {

   if(EcuabFee_Ih_ReadBuffer[LoopCounter] != EcuabFee_Ph_GetFlsEraseValue())
   {
      retVal = EcuabFee_PATTERN_INCONSISTENT;
      break;
   }
  }

  return retVal;
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckStartPattern(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return EcuabFee_Fls_BlankCheck(EcuabFee_Ih_GetInstanceStartAddress(Instance), EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckEndPattern(EcuabFee_Ih_InstanceConstPointerType Instance)
{
  return EcuabFee_Fls_BlankCheck(EcuabFee_Ih_GetEndPatternAddress(Instance), EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_BlankCheckLastPage(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  return EcuabFee_Fls_BlankCheck(EcuabFee_Ih_GetEndAddress(Instance) - EcuabFee_Ph_GetPartitionWriteAlignment(), EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadLastPage(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  return EcuabFee_Fls_Read(EcuabFee_Ih_GetEndAddress(Instance) - EcuabFee_Ph_GetPartitionWriteAlignment(), EcuabFee_Ih_ReadBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadAllMgmt(EcuabFee_Ih_InstanceConstPointerType Instance)
{

  uint16 readLength = (uint16) EcuabFee_AlignValue( ((uint32)(2 * EcuabFee_Ph_GetPartitionWriteAlignment()) + EcuabFee_MANAGEMENT_SIZE), EcuabFee_Ph_GetPartitionReadAlignment() );
  return EcuabFee_Fls_Read(EcuabFee_Ih_GetInstanceStartAddress(Instance), EcuabFee_Ih_ReadBuffer, readLength);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_ReadData(EcuabFee_Ih_InstanceConstPointerType Instance
   ,                                                   EcuabFee_Ih_BufferPointerType DataBufferPtr
   ,                                                   uint16 BlockOffset
   ,                                                   uint16 ReadLength)
{

  EcuabFee_AddressType readAddress = EcuabFee_Ih_GetManagementAddress(Instance) + EcuabFee_MANAGEMENT_SIZE + BlockOffset;
  return EcuabFee_Fls_Read(readAddress, DataBufferPtr, ReadLength);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteStartPattern(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_Ih_CompareParameterPointerType CompareParameter)
{

  EcuabFee_Ih_FillBufferWithStaticPattern();

  CompareParameter->EcuabFee_Ih_CompareAddress = EcuabFee_Ih_GetInstanceStartAddress(Instance);
  CompareParameter->EcuabFee_Ih_CompareBufferPtr = EcuabFee_Ih_WriteBuffer;
  CompareParameter->EcuabFee_Ih_CompareLength = EcuabFee_Ph_GetPartitionWriteAlignment();

  return EcuabFee_Fls_Write(EcuabFee_Ih_GetInstanceStartAddress(Instance), EcuabFee_Ih_WriteBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteManagementPage(EcuabFee_Ih_InstanceConstPointerType Instance
   ,       EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 FillLength = EcuabFee_Ph_GetPartitionWriteAlignment() - (uint16) EcuabFee_MANAGEMENT_SIZE;

  EcuabFee_Ih_FillBufferWithErasedValue();

  EcuabFee_Ih_WriteBuffer[0] = EcuabFee_Ih_GetMgmtValue(Instance);

  if(EcuabFee_Ih_GetInstanceDataLength(Instance) < FillLength)
  {
    FillLength = EcuabFee_Ih_GetInstanceDataLength(Instance);
  }

  if(UserJobParameter.JobType == EcuabFee_WRITE_JOB)
  {
   for(LoopCounter = 0; LoopCounter < FillLength; LoopCounter++)
   {
      EcuabFee_Ih_WriteBuffer[EcuabFee_MANAGEMENT_SIZE + LoopCounter] = UserJobParameter.DataBufferPtr[LoopCounter];
   }
  }

  CompareParameter->EcuabFee_Ih_CompareAddress = EcuabFee_Ih_GetManagementAddress(Instance);
  CompareParameter->EcuabFee_Ih_CompareBufferPtr = EcuabFee_Ih_WriteBuffer;
  CompareParameter->EcuabFee_Ih_CompareLength = EcuabFee_Ph_GetPartitionWriteAlignment();

  return EcuabFee_Fls_Write(EcuabFee_Ih_GetManagementAddress(Instance), EcuabFee_Ih_WriteBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WritePayload(EcuabFee_Ih_InstanceConstPointerType Instance
   ,       EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 pageSize = EcuabFee_Ph_GetPartitionWriteAlignment();

  EcuabFee_AddressType startAddress = EcuabFee_Ih_GetManagementAddress(Instance) + pageSize;

  uint16 BufferIndex = (uint16) (pageSize - EcuabFee_MANAGEMENT_SIZE);

  uint16 length = ((EcuabFee_Ih_GetInstanceDataLength(Instance) - BufferIndex) / pageSize) * pageSize;

  CompareParameter->EcuabFee_Ih_CompareAddress = startAddress;
  CompareParameter->EcuabFee_Ih_CompareBufferPtr = &(UserJobParameter.DataBufferPtr[BufferIndex]);
  CompareParameter->EcuabFee_Ih_CompareLength = length;

  return EcuabFee_Fls_Write(startAddress, &(UserJobParameter.DataBufferPtr[BufferIndex]), length );
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteLastPage(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_UserJobParameterType UserJobParameter, EcuabFee_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 BufferIndex;
  EcuabFee_AddressType startAddress;
  uint16 PageSize = EcuabFee_Ph_GetPartitionWriteAlignment();

  uint16 ResidualLength = (uint16) (EcuabFee_Ih_GetInstanceDataLength(Instance) +  EcuabFee_MANAGEMENT_SIZE) % PageSize;

  if(ResidualLength == 0)
  {

    ResidualLength = PageSize;
  }

  BufferIndex = EcuabFee_Ih_GetInstanceDataLength(Instance) - ResidualLength;

  startAddress = EcuabFee_Ih_GetManagementAddress(Instance) + EcuabFee_MANAGEMENT_SIZE + BufferIndex;

  EcuabFee_Ih_FillBufferWithErasedValue();

  for(LoopCounter = 0; LoopCounter < ResidualLength; LoopCounter++)
  {
    EcuabFee_Ih_WriteBuffer[LoopCounter] = UserJobParameter.DataBufferPtr[BufferIndex];
    BufferIndex++;
  }

  CompareParameter->EcuabFee_Ih_CompareAddress = startAddress;
  CompareParameter->EcuabFee_Ih_CompareBufferPtr = EcuabFee_Ih_WriteBuffer;
  CompareParameter->EcuabFee_Ih_CompareLength = PageSize;

  return EcuabFee_Fls_Write(startAddress, EcuabFee_Ih_WriteBuffer, PageSize);
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_WriteEndPattern(EcuabFee_Ih_InstanceConstPointerType Instance
   ,   EcuabFee_Ih_CompareParameterPointerType CompareParameter)
{

  EcuabFee_Ih_FillBufferWithStaticPattern();

  CompareParameter->EcuabFee_Ih_CompareAddress = EcuabFee_Ih_GetEndPatternAddress(Instance);
  CompareParameter->EcuabFee_Ih_CompareBufferPtr = EcuabFee_Ih_WriteBuffer;
  CompareParameter->EcuabFee_Ih_CompareLength = EcuabFee_Ph_GetPartitionWriteAlignment();

  return EcuabFee_Fls_Write(EcuabFee_Ih_GetEndPatternAddress(Instance), EcuabFee_Ih_WriteBuffer, EcuabFee_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Ih_VerifyData(EcuabFee_Ih_CompareParameterType CompareParameter)
{
  return EcuabFee_Fls_Compare(CompareParameter.EcuabFee_Ih_CompareAddress, CompareParameter.EcuabFee_Ih_CompareBufferPtr
   ,         CompareParameter.EcuabFee_Ih_CompareLength);
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

