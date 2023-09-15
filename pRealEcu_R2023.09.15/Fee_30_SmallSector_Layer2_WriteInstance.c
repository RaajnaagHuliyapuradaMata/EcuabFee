#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_Layer2_WriteInstance.hpp"

#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"

#include "Fee_30_SmallSector_DatasetHandler.hpp"
#include "Fee_30_SmallSector_BlockHandler.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"

#define FEE_30_SMALLSECTOR_WRITE_INSTANCE_LAYER FEE_30_SMALLSECTOR_LAYER_TWO_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
  FEE_30_SMALLSECTOR_WRINST_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_START_PATTERN
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_START_PATTERN
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_MGMT_PAGE
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_MGMT_PAGE
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_PAYLOAD
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_PAYLOAD
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_LAST_PAGE
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_LAST_PAGE
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_END_PATTERN
   ,  FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_END_PATTERN
}Fee_30_SmallSector_WrInst_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  Fee_30_SmallSector_WrInst_StateType StateMachine;
}Fee_30_SmallSector_WrInst_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_WrInst_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_WrInst_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_WRINST_STATE_UNINIT };

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Ih_CompareParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_WrInst_CompareParameter;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Ih_InstanceType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_WrInst_Instance;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_UserJobParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_WrInst_UserJobParameter;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_PrepareInstanceForWriting(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_InitiateVerifyState(Fee_30_SmallSector_WrInst_StateType nextState);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_StartManagementPageWrite(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_InitiateNextWriteState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_WriteLastPageOrEndPattern(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_StartEndPatternWrite(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateIdle(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteStartPattern(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteManagementPage(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWritePayload(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteLastPage(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteEndPattern(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_WrInst_Execute, Fee_30_SmallSector_WrInst_Cancel, FEE_30_SMALLSECTOR_WRITE_INSTANCE_LAYER);

  Fee_30_SmallSector_WrInst_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_IDLE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_PrepareInstanceForWriting(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{

  if(Fee_30_SmallSector_WrInst_UserJobParameter.JobType == FEE_30_SMALLSECTOR_WRITE_JOB)
  {
      Fee_30_SmallSector_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_OK);
  }
  else{
    Fee_30_SmallSector_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_INVALID);
  }

  Fee_30_SmallSector_Ih_SetManagementValuesAccordingToStatus(Instance);
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_InitiateVerifyState(Fee_30_SmallSector_WrInst_StateType nextState)
{
  if(Fee_30_SmallSector_Ih_VerifyData(Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
  {
    Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = nextState;
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_StartManagementPageWrite(void){
  if(Fee_30_SmallSector_Ih_WriteManagementPage(&Fee_30_SmallSector_WrInst_Instance, Fee_30_SmallSector_WrInst_UserJobParameter, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
  {
    Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_MGMT_PAGE;
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_InitiateNextWriteState(void){

  uint16 PageSize = Fee_30_SmallSector_Ph_GetPartitionWriteAlignment();
  uint16 MgmtSize = (uint16) FEE_30_SMALLSECTOR_MANAGEMENT_SIZE;
  uint16 PayloadLength = Fee_30_SmallSector_Ih_GetInstanceDataLength(&Fee_30_SmallSector_WrInst_Instance);

  if((PayloadLength <= (PageSize - MgmtSize)) || (Fee_30_SmallSector_WrInst_UserJobParameter.JobType != FEE_30_SMALLSECTOR_WRITE_JOB))
  {

    Fee_30_SmallSector_WrInst_StartEndPatternWrite();
  }

  else if(PayloadLength > ((2u * PageSize) - MgmtSize))
  {

   if(Fee_30_SmallSector_Ih_WritePayload(&Fee_30_SmallSector_WrInst_Instance, Fee_30_SmallSector_WrInst_UserJobParameter, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
   {
      Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_PAYLOAD;
   }
   else{
      Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }

  else{

   if(Fee_30_SmallSector_Ih_WriteLastPage(&Fee_30_SmallSector_WrInst_Instance, Fee_30_SmallSector_WrInst_UserJobParameter, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
   {
      Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_LAST_PAGE;
   }
   else{
      Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_WriteLastPageOrEndPattern(void){

  if(Fee_30_SmallSector_Ih_IsLastPageNecessary(&Fee_30_SmallSector_WrInst_Instance) == TRUE)
  {

   if(Fee_30_SmallSector_Ih_WriteLastPage(&Fee_30_SmallSector_WrInst_Instance, Fee_30_SmallSector_WrInst_UserJobParameter, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
   {
      Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_LAST_PAGE;
   }
   else{
      Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

    Fee_30_SmallSector_WrInst_StartEndPatternWrite();
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_StartEndPatternWrite(void){
  if(Fee_30_SmallSector_Ih_WriteEndPattern(&Fee_30_SmallSector_WrInst_Instance, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
  {
    Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_END_PATTERN;
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateIdle(void){

  Fee_30_SmallSector_WrInst_PrepareInstanceForWriting(&Fee_30_SmallSector_WrInst_Instance);

  if(Fee_30_SmallSector_Ih_WriteStartPattern(&Fee_30_SmallSector_WrInst_Instance, &Fee_30_SmallSector_WrInst_CompareParameter) == E_OK)
  {
    Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_START_PATTERN;
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteStartPattern(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Bh_HasVerificationEnabled() == TRUE)
   {
      Fee_30_SmallSector_WrInst_InitiateVerifyState(FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_START_PATTERN);
   }
   else{
      Fee_30_SmallSector_WrInst_StartManagementPageWrite();
   }
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteManagementPage(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Bh_HasVerificationEnabled() == TRUE)
   {
      Fee_30_SmallSector_WrInst_InitiateVerifyState(FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_MGMT_PAGE);
   }
   else{
      Fee_30_SmallSector_WrInst_InitiateNextWriteState();
   }
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWritePayload(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Bh_HasVerificationEnabled() == TRUE)
   {
      Fee_30_SmallSector_WrInst_InitiateVerifyState(FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_PAYLOAD);
   }
   else{

      Fee_30_SmallSector_WrInst_WriteLastPageOrEndPattern();
   }
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteLastPage(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Bh_HasVerificationEnabled() == TRUE)
   {
      Fee_30_SmallSector_WrInst_InitiateVerifyState(FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_LAST_PAGE);
   }
   else{

      Fee_30_SmallSector_WrInst_StartEndPatternWrite();
   }
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateWriteEndPattern(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Bh_HasVerificationEnabled() == TRUE)
   {
      Fee_30_SmallSector_WrInst_InitiateVerifyState(FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_END_PATTERN);
   }
   else{
      Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
   }
  }
  else{
    Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_WrInst_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_WRINST_STATE_IDLE:
      Fee_30_SmallSector_WrInst_ProcessStateIdle();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_START_PATTERN:
      Fee_30_SmallSector_WrInst_ProcessStateWriteStartPattern();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_START_PATTERN:
      if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        Fee_30_SmallSector_WrInst_StartManagementPageWrite();
      }
      else{
        Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      }
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_MGMT_PAGE:
      Fee_30_SmallSector_WrInst_ProcessStateWriteManagementPage();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_MGMT_PAGE:
      if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        Fee_30_SmallSector_WrInst_InitiateNextWriteState();
      }
      else{
        Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      }
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_PAYLOAD:
      Fee_30_SmallSector_WrInst_ProcessStateWritePayload();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_PAYLOAD:
      if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        Fee_30_SmallSector_WrInst_WriteLastPageOrEndPattern();
      }
      else{
        Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      }
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_LAST_PAGE:
      Fee_30_SmallSector_WrInst_ProcessStateWriteLastPage();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_LAST_PAGE:
      if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        Fee_30_SmallSector_WrInst_StartEndPatternWrite();
      }
      else{
        Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      }
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_WRITE_END_PATTERN:
      Fee_30_SmallSector_WrInst_ProcessStateWriteEndPattern();
      break;

   case FEE_30_SMALLSECTOR_WRINST_STATE_VERIFY_END_PATTERN:
        Fee_30_SmallSector_WrInst_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      break;

    default:
      Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Init(void){
  Fee_30_SmallSector_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_WrInst_ComponentParameter.JobResult = MEMIF_JOB_OK;

  Fee_30_SmallSector_WrInst_CompareParameter.Fee_30_SmallSector_Ih_CompareAddress = 0x0000U;
  Fee_30_SmallSector_WrInst_CompareParameter.Fee_30_SmallSector_Ih_CompareBufferPtr = NULL_PTR;
  Fee_30_SmallSector_WrInst_CompareParameter.Fee_30_SmallSector_Ih_CompareLength = 0x0000U;

  Fee_30_SmallSector_Ih_InitInstance(&Fee_30_SmallSector_WrInst_Instance);

  Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WRINST_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_GetStatus(void){
  return Fee_30_SmallSector_WrInst_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_GetJobResult(void){
  return Fee_30_SmallSector_WrInst_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_StartJob(Fee_30_SmallSector_Ih_InstanceType Instance
   ,     Fee_30_SmallSector_UserJobParameterType UserJobParameter)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_WrInst_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_WRINST_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_WrInst_Execute, Fee_30_SmallSector_WrInst_Cancel, FEE_30_SMALLSECTOR_WRITE_INSTANCE_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_WrInst_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_WrInst_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      Fee_30_SmallSector_WrInst_Instance = Instance;
      Fee_30_SmallSector_WrInst_UserJobParameter = UserJobParameter;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Execute(void){

  if(Fee_30_SmallSector_WrInst_ComponentParameter.Status == MEMIF_BUSY)
  {
    Fee_30_SmallSector_WrInst_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_WrInst_Cancel(void){
  Fee_30_SmallSector_WrInst_FinishJob(MEMIF_JOB_CANCELED);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

