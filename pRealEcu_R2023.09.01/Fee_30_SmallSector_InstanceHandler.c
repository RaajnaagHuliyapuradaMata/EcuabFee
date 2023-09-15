

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_InstanceHandler.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"

#define FEE_30_SMALLSECTOR_CYCLE_COUNTER_BIT_MASK  (0x3U)

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(uint8, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Ih_ReadBuffer[FEE_30_SMALLSECTOR_MAX_READ_SIZE];
FEE_30_SMALLSECTOR_LOCAL VAR(uint8, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Ih_WriteBuffer[FEE_30_SMALLSECTOR_MAX_WRITE_ALIGNMENT];

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CONST_8BIT
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL CONST(uint8, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Ih_ValidManagementValue = 0xF0u;
FEE_30_SMALLSECTOR_LOCAL CONST(uint8, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Ih_InvalidManagementValue = 0x0Fu;

#define FEE_30_SMALLSECTOR_STOP_SEC_CONST_8BIT
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_IsValidManagement(uint8 ManagementByte);

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_IsInvalidManagement(uint8 ManagementByte);

#if(FEE_30_SMALLSECTOR_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_HasEvenParity(uint8 ManagementNibble);
#endif

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_FillBufferWithStaticPattern(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_FillBufferWithErasedValue(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_IsValidManagement(uint8 ManagementByte)
{
  return (ManagementByte == Fee_30_SmallSector_Ih_ValidManagementValue) ? TRUE : FALSE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_IsInvalidManagement(uint8 ManagementByte)
{
  return (ManagementByte == Fee_30_SmallSector_Ih_InvalidManagementValue) ? TRUE : FALSE;
}

#if(FEE_30_SMALLSECTOR_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_HasEvenParity(uint8 ManagementNibble)
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

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_FillBufferWithStaticPattern(void){
  uint8 LoopCounter;

  for(LoopCounter = 0u; LoopCounter < FEE_30_SMALLSECTOR_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    Fee_30_SmallSector_Ih_WriteBuffer[LoopCounter] = FEE_30_SMALLSECTOR_STATIC_PATTERN;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Ih_FillBufferWithErasedValue(void){
  uint8 LoopCounter;

  for(LoopCounter = 0u; LoopCounter < FEE_30_SMALLSECTOR_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    Fee_30_SmallSector_Ih_WriteBuffer[LoopCounter] = Fee_30_SmallSector_Ph_GetFlsEraseValue();
  }
}

FUNC(boolean, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_IsLastPageNecessary(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  return (boolean) ((Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE) % Fee_30_SmallSector_Ph_GetPartitionWriteAlignment()) != 0;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_CreateInstance(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance
   ,       Fee_30_SmallSector_AddressType InstanceStartAddress, uint16 DataLength)
{
  Instance->StartAddress = InstanceStartAddress;
  Instance->WP2Address = InstanceStartAddress + Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();
  Instance->MgmtAddress = InstanceStartAddress + (Fee_30_SmallSector_AddressType) (2 * Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
  Instance->DataLength = DataLength;
  Instance->MgmtValue = Fee_30_SmallSector_Ph_GetFlsEraseValue();
  Instance->WP1State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->WP2State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->LastPageState = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_InitInstance(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{
  Instance->StartAddress = 0x0000U;
  Instance->WP2Address = 0x0000U;
  Instance->MgmtAddress = 0x0000U;
  Instance->DataLength = 0x0000U;
  Instance->WP1State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->WP2State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->MgmtValue = 0x00U;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetInstanceStartAddress(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->StartAddress;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetEndPatternAddress(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP2Address;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetManagementAddress(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->MgmtAddress;
}

FUNC (Fee_30_SmallSector_AddressType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetEndAddress(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  return Fee_30_SmallSector_AlignValue(Fee_30_SmallSector_Ih_GetManagementAddress(Instance) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE + Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance)
   ,     Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (uint16, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetInstanceDataLength(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->DataLength;
}

FUNC (Fee_30_SmallSector_Ih_InstanceStatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetInstanceStatus(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->InstanceStatus;
}

FUNC(uint8, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetMgmtValue(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->MgmtValue;
}

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetWP1State(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP1State;
}

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetWP2State(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->WP2State;
}

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetLastPageState(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Instance->LastPageState;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_StoreMgmtValueFromBuffer(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{

  Instance->MgmtValue = Fee_30_SmallSector_Ih_ReadBuffer[2 * Fee_30_SmallSector_Ph_GetPartitionWriteAlignment()];
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetInstanceStatus(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance, Fee_30_SmallSector_Ih_InstanceStatusType status)
{
  Instance->InstanceStatus = status;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetWP1State(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance, Fee_30_SmallSector_StaticPatternState state)
{
  Instance->WP1State = state;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetWP2State(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance, Fee_30_SmallSector_StaticPatternState state)
{
  Instance->WP2State = state;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetLastPageState(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance, Fee_30_SmallSector_StaticPatternState state)
{
  Instance->LastPageState = state;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_PrepareNextInstanceProperties(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance
   ,       Fee_30_SmallSector_AddressType StartAddress)
{

  Instance->StartAddress = StartAddress;
  Instance->WP2Address = StartAddress + Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();
  Instance->MgmtAddress = StartAddress + (Fee_30_SmallSector_AddressType) (2 * Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());

  Instance->MgmtValue = Fee_30_SmallSector_Ph_GetFlsEraseValue();
  Instance->WP1State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->WP2State = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->LastPageState = FEE_30_SMALLSECTOR_PATTERN_INIT;
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetManagementValuesAccordingToStatus(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{

  if(Fee_30_SmallSector_Ih_GetInstanceStatus(Instance) == INSTANCE_STATUS_OK)
  {

    Instance->MgmtValue = Fee_30_SmallSector_Ih_ValidManagementValue;
  }
  else{

    Instance->MgmtValue = Fee_30_SmallSector_Ih_InvalidManagementValue;
  }
}

FUNC (MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_SetNegativeJobResultAccordingToStatus(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  MemIf_JobResultType retVal;

  switch(Fee_30_SmallSector_Ih_GetInstanceStatus(Instance))
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

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_DetermineInstanceStatus(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{
  Fee_30_SmallSector_Ih_InstanceStatusType status;
  uint8 mgmtValue = Fee_30_SmallSector_Ih_GetMgmtValue(Instance);
  Fee_30_SmallSector_StaticPatternState wp1State = Fee_30_SmallSector_Ih_GetWP1State(Instance);
  Fee_30_SmallSector_StaticPatternState wp2State = Fee_30_SmallSector_Ih_GetWP2State(Instance);
  Fee_30_SmallSector_StaticPatternState lastPageState = Fee_30_SmallSector_Ih_GetLastPageState(Instance);

  if((wp1State == FEE_30_SMALLSECTOR_PATTERN_BLANK) &&
      (wp2State == FEE_30_SMALLSECTOR_PATTERN_BLANK) &&
      (lastPageState == FEE_30_SMALLSECTOR_PATTERN_BLANK)
  )
  {
   status = INSTANCE_STATUS_ERASED;
  }

  else if( (wp1State == FEE_30_SMALLSECTOR_PATTERN_VALID) &&
            (wp2State == FEE_30_SMALLSECTOR_PATTERN_VALID) )
  {

   if((Fee_30_SmallSector_Ih_IsValidManagement(mgmtValue) == TRUE) &&
        (lastPageState != FEE_30_SMALLSECTOR_PATTERN_BLANK))
   {
      status = INSTANCE_STATUS_OK;
   }

   else if(Fee_30_SmallSector_Ih_IsInvalidManagement(mgmtValue) == TRUE)
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

#if(FEE_30_SMALLSECTOR_CORRECT_SINGLE_BIT_FLIPS == STD_ON)

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_CorrectSingleBitFlips(Fee_30_SmallSector_Ih_BufferPointerType ManagementByte)
{
  uint8 correctNibble;
  uint8 LowerNibble = (*ManagementByte) & 0x0Fu;
  uint8 HigherNibbleInverted = ((   (uint8)( ~(*ManagementByte) )   ) >> 4u) & 0x0Fu;
  uint8 XorLowerHigherNibble = LowerNibble ^ HigherNibbleInverted;

  if(LowerNibble != HigherNibbleInverted)
  {

   if((XorLowerHigherNibble & (XorLowerHigherNibble - 1u)) == 0u)
   {

      if(Fee_30_SmallSector_Ih_HasEvenParity(LowerNibble) == TRUE)
      {
        correctNibble = LowerNibble;
      }
      else{
        correctNibble = HigherNibbleInverted;
      }

      if((correctNibble & 4u) == 0u)
      {

        *ManagementByte = Fee_30_SmallSector_Ih_ValidManagementValue;
      }
      else{

        *ManagementByte = Fee_30_SmallSector_Ih_InvalidManagementValue;
      }
   }
  }
}
#endif

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetStartPatternFromBuffer(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  uint16 LoopCounter;
  Fee_30_SmallSector_StaticPatternState retVal = FEE_30_SMALLSECTOR_PATTERN_INIT;
  boolean isPatternValid = FALSE;
  boolean isPatternBlank = FALSE;

  for(LoopCounter = 0; LoopCounter < Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {

   if(Fee_30_SmallSector_Ih_ReadBuffer[LoopCounter] == FEE_30_SMALLSECTOR_STATIC_PATTERN)
   {
      isPatternValid = TRUE;
      retVal = FEE_30_SMALLSECTOR_PATTERN_VALID;
   }

   else if(Fee_30_SmallSector_Ih_ReadBuffer[LoopCounter] == Fee_30_SmallSector_Ph_GetFlsEraseValue())
   {
      isPatternBlank = TRUE;
      retVal = FEE_30_SMALLSECTOR_PATTERN_BLANK;
   }
   else{

      isPatternBlank = TRUE;
      isPatternValid = TRUE;
      break;
   }
  }

  if((isPatternValid ^ isPatternBlank) == FALSE)
  {
    retVal = FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT;
  }

  if(Fee_30_SmallSector_Ih_GetWP1State(Instance) == FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT)
  {
   if(retVal == FEE_30_SMALLSECTOR_PATTERN_BLANK)
   {

      retVal = FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT;
   }

  }

  return retVal;
}

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetEndPatternFromBuffer(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  uint16 LoopCounter;
  Fee_30_SmallSector_StaticPatternState retVal = FEE_30_SMALLSECTOR_PATTERN_INIT;
  boolean isPatternValid = FALSE;
  boolean isPatternBlank = FALSE;

  for(LoopCounter = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(); LoopCounter < (2 * Fee_30_SmallSector_Ph_GetPartitionWriteAlignment()); LoopCounter++)
  {

   if(Fee_30_SmallSector_Ih_ReadBuffer[LoopCounter] == FEE_30_SMALLSECTOR_STATIC_PATTERN)
   {
      isPatternValid = TRUE;
      retVal = FEE_30_SMALLSECTOR_PATTERN_VALID;
   }

   else if(Fee_30_SmallSector_Ih_ReadBuffer[LoopCounter] == Fee_30_SmallSector_Ph_GetFlsEraseValue())
   {
      isPatternBlank = TRUE;
      retVal = FEE_30_SMALLSECTOR_PATTERN_BLANK;
   }
   else{

      isPatternBlank = TRUE;
      isPatternValid = TRUE;
      break;
   }
  }

  if((isPatternValid ^ isPatternBlank) == FALSE)
  {
    retVal = FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT;
  }

  if(Fee_30_SmallSector_Ih_GetWP2State(Instance) == FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT)
  {
   if(retVal == FEE_30_SMALLSECTOR_PATTERN_BLANK)
   {

      retVal = FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT;
   }

  }

  return retVal;
}

FUNC (Fee_30_SmallSector_StaticPatternState, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_GetPageStatusFromBuffer(void){
  uint16 LoopCounter;
  Fee_30_SmallSector_StaticPatternState retVal = FEE_30_SMALLSECTOR_PATTERN_BLANK;

  for(LoopCounter = 0; LoopCounter < Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(); LoopCounter++)
  {

   if(Fee_30_SmallSector_Ih_ReadBuffer[LoopCounter] != Fee_30_SmallSector_Ph_GetFlsEraseValue())
   {
      retVal = FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT;
      break;
   }
  }

  return retVal;
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_BlankCheckStartPattern(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Fee_30_SmallSector_Fls_BlankCheck(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance), Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_BlankCheckEndPattern(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{
  return Fee_30_SmallSector_Fls_BlankCheck(Fee_30_SmallSector_Ih_GetEndPatternAddress(Instance), Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_BlankCheckLastPage(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  return Fee_30_SmallSector_Fls_BlankCheck(Fee_30_SmallSector_Ih_GetEndAddress(Instance) - Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(), Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_ReadLastPage(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  return Fee_30_SmallSector_Fls_Read(Fee_30_SmallSector_Ih_GetEndAddress(Instance) - Fee_30_SmallSector_Ph_GetPartitionWriteAlignment(), Fee_30_SmallSector_Ih_ReadBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_ReadAllMgmt(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance)
{

  uint16 readLength = (uint16) Fee_30_SmallSector_AlignValue( ((uint32)(2 * Fee_30_SmallSector_Ph_GetPartitionWriteAlignment()) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE), Fee_30_SmallSector_Ph_GetPartitionReadAlignment() );
  return Fee_30_SmallSector_Fls_Read(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance), Fee_30_SmallSector_Ih_ReadBuffer, readLength);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_ReadData(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,                                                   Fee_30_SmallSector_Ih_BufferPointerType DataBufferPtr
   ,                                                   uint16 BlockOffset
   ,                                                   uint16 ReadLength)
{

  Fee_30_SmallSector_AddressType readAddress = Fee_30_SmallSector_Ih_GetManagementAddress(Instance) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE + BlockOffset;
  return Fee_30_SmallSector_Fls_Read(readAddress, DataBufferPtr, ReadLength);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_WriteStartPattern(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,   Fee_30_SmallSector_Ih_CompareParameterPointerType CompareParameter)
{

  Fee_30_SmallSector_Ih_FillBufferWithStaticPattern();

  CompareParameter->Fee_30_SmallSector_Ih_CompareAddress = Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance);
  CompareParameter->Fee_30_SmallSector_Ih_CompareBufferPtr = Fee_30_SmallSector_Ih_WriteBuffer;
  CompareParameter->Fee_30_SmallSector_Ih_CompareLength = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  return Fee_30_SmallSector_Fls_Write(Fee_30_SmallSector_Ih_GetInstanceStartAddress(Instance), Fee_30_SmallSector_Ih_WriteBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_WriteManagementPage(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,       Fee_30_SmallSector_UserJobParameterType UserJobParameter, Fee_30_SmallSector_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 FillLength = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment() - (uint16) FEE_30_SMALLSECTOR_MANAGEMENT_SIZE;

  Fee_30_SmallSector_Ih_FillBufferWithErasedValue();

  Fee_30_SmallSector_Ih_WriteBuffer[0] = Fee_30_SmallSector_Ih_GetMgmtValue(Instance);

  if(Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance) < FillLength)
  {
    FillLength = Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance);
  }

  if(UserJobParameter.JobType == FEE_30_SMALLSECTOR_WRITE_JOB)
  {
   for(LoopCounter = 0; LoopCounter < FillLength; LoopCounter++)
   {
      Fee_30_SmallSector_Ih_WriteBuffer[FEE_30_SMALLSECTOR_MANAGEMENT_SIZE + LoopCounter] = UserJobParameter.DataBufferPtr[LoopCounter];
   }
  }

  CompareParameter->Fee_30_SmallSector_Ih_CompareAddress = Fee_30_SmallSector_Ih_GetManagementAddress(Instance);
  CompareParameter->Fee_30_SmallSector_Ih_CompareBufferPtr = Fee_30_SmallSector_Ih_WriteBuffer;
  CompareParameter->Fee_30_SmallSector_Ih_CompareLength = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  return Fee_30_SmallSector_Fls_Write(Fee_30_SmallSector_Ih_GetManagementAddress(Instance), Fee_30_SmallSector_Ih_WriteBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_WritePayload(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,       Fee_30_SmallSector_UserJobParameterType UserJobParameter, Fee_30_SmallSector_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 pageSize = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  Fee_30_SmallSector_AddressType startAddress = Fee_30_SmallSector_Ih_GetManagementAddress(Instance) + pageSize;

  uint16 BufferIndex = (uint16) (pageSize - FEE_30_SMALLSECTOR_MANAGEMENT_SIZE);

  uint16 length = ((Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance) - BufferIndex) / pageSize) * pageSize;

  CompareParameter->Fee_30_SmallSector_Ih_CompareAddress = startAddress;
  CompareParameter->Fee_30_SmallSector_Ih_CompareBufferPtr = &(UserJobParameter.DataBufferPtr[BufferIndex]);
  CompareParameter->Fee_30_SmallSector_Ih_CompareLength = length;

  return Fee_30_SmallSector_Fls_Write(startAddress, &(UserJobParameter.DataBufferPtr[BufferIndex]), length );
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_WriteLastPage(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,   Fee_30_SmallSector_UserJobParameterType UserJobParameter, Fee_30_SmallSector_Ih_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 BufferIndex;
  Fee_30_SmallSector_AddressType startAddress;
  uint16 PageSize = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  uint16 ResidualLength = (uint16) (Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance) +  FEE_30_SMALLSECTOR_MANAGEMENT_SIZE) % PageSize;

  if(ResidualLength == 0)
  {

    ResidualLength = PageSize;
  }

  BufferIndex = Fee_30_SmallSector_Ih_GetInstanceDataLength(Instance) - ResidualLength;

  startAddress = Fee_30_SmallSector_Ih_GetManagementAddress(Instance) + FEE_30_SMALLSECTOR_MANAGEMENT_SIZE + BufferIndex;

  Fee_30_SmallSector_Ih_FillBufferWithErasedValue();

  for(LoopCounter = 0; LoopCounter < ResidualLength; LoopCounter++)
  {
    Fee_30_SmallSector_Ih_WriteBuffer[LoopCounter] = UserJobParameter.DataBufferPtr[BufferIndex];
    BufferIndex++;
  }

  CompareParameter->Fee_30_SmallSector_Ih_CompareAddress = startAddress;
  CompareParameter->Fee_30_SmallSector_Ih_CompareBufferPtr = Fee_30_SmallSector_Ih_WriteBuffer;
  CompareParameter->Fee_30_SmallSector_Ih_CompareLength = PageSize;

  return Fee_30_SmallSector_Fls_Write(startAddress, Fee_30_SmallSector_Ih_WriteBuffer, PageSize);
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_WriteEndPattern(Fee_30_SmallSector_Ih_InstanceConstPointerType Instance
   ,   Fee_30_SmallSector_Ih_CompareParameterPointerType CompareParameter)
{

  Fee_30_SmallSector_Ih_FillBufferWithStaticPattern();

  CompareParameter->Fee_30_SmallSector_Ih_CompareAddress = Fee_30_SmallSector_Ih_GetEndPatternAddress(Instance);
  CompareParameter->Fee_30_SmallSector_Ih_CompareBufferPtr = Fee_30_SmallSector_Ih_WriteBuffer;
  CompareParameter->Fee_30_SmallSector_Ih_CompareLength = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();

  return Fee_30_SmallSector_Fls_Write(Fee_30_SmallSector_Ih_GetEndPatternAddress(Instance), Fee_30_SmallSector_Ih_WriteBuffer, Fee_30_SmallSector_Ph_GetPartitionWriteAlignment());
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Ih_VerifyData(Fee_30_SmallSector_Ih_CompareParameterType CompareParameter)
{
  return Fee_30_SmallSector_Fls_Compare(CompareParameter.Fee_30_SmallSector_Ih_CompareAddress, CompareParameter.Fee_30_SmallSector_Ih_CompareBufferPtr
   ,         CompareParameter.Fee_30_SmallSector_Ih_CompareLength);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

