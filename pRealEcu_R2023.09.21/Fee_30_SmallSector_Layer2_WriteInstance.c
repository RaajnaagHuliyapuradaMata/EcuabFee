#include "Std_Types.hpp"

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer2_WriteInstance.hpp"

#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_BlockHandler.hpp"
#include "EcuabFee_PartitionHandler.hpp"

#define EcuabFee_WRITE_INSTANCE_LAYER EcuabFee_LAYER_TWO_INDEX

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef enum{
  EcuabFee_WRINST_STATE_UNINIT = 0
   ,  EcuabFee_WRINST_STATE_IDLE
   ,  EcuabFee_WRINST_STATE_WRITE_START_PATTERN
   ,  EcuabFee_WRINST_STATE_VERIFY_START_PATTERN
   ,  EcuabFee_WRINST_STATE_WRITE_MGMT_PAGE
   ,  EcuabFee_WRINST_STATE_VERIFY_MGMT_PAGE
   ,  EcuabFee_WRINST_STATE_WRITE_PAYLOAD
   ,  EcuabFee_WRINST_STATE_VERIFY_PAYLOAD
   ,  EcuabFee_WRINST_STATE_WRITE_LAST_PAGE
   ,  EcuabFee_WRINST_STATE_VERIFY_LAST_PAGE
   ,  EcuabFee_WRINST_STATE_WRITE_END_PATTERN
   ,  EcuabFee_WRINST_STATE_VERIFY_END_PATTERN
}EcuabFee_WrInst_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_WrInst_StateType StateMachine;
}EcuabFee_WrInst_ComponentParameterType;

#define EcuabFee_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_WrInst_ComponentParameterType, EcuabFee_APPL_DATA) EcuabFee_WrInst_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EcuabFee_WRINST_STATE_UNINIT };

#define EcuabFee_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Ih_CompareParameterType, EcuabFee_APPL_DATA) EcuabFee_WrInst_CompareParameter;

EcuabFee_LOCAL VAR(EcuabFee_Ih_InstanceType, EcuabFee_APPL_DATA) EcuabFee_WrInst_Instance;

EcuabFee_LOCAL VAR(EcuabFee_UserJobParameterType, EcuabFee_APPL_DATA) EcuabFee_WrInst_UserJobParameter;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_FinishJob(MemIf_JobResultType JobResult);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_PrepareInstanceForWriting(EcuabFee_Ih_InstanceVarPointerType Instance);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WrInst_StateType nextState);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_StartManagementPageWrite(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_InitiateNextWriteState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_WriteLastPageOrEndPattern(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_StartEndPatternWrite(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateIdle(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteStartPattern(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteManagementPage(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWritePayload(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteLastPage(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteEndPattern(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateMachine(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_WrInst_Execute, EcuabFee_WrInst_Cancel, EcuabFee_WRITE_INSTANCE_LAYER);

  EcuabFee_WrInst_ComponentParameter.JobResult = JobResult;
  EcuabFee_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_IDLE;
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_PrepareInstanceForWriting(EcuabFee_Ih_InstanceVarPointerType Instance)
{

  if(EcuabFee_WrInst_UserJobParameter.JobType == EcuabFee_WRITE_JOB)
  {
      EcuabFee_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_OK);
  }
  else{
    EcuabFee_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_INVALID);
  }

  EcuabFee_Ih_SetManagementValuesAccordingToStatus(Instance);
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WrInst_StateType nextState)
{
  if(EcuabFee_Ih_VerifyData(EcuabFee_WrInst_CompareParameter) == E_OK)
  {
    EcuabFee_WrInst_ComponentParameter.StateMachine = nextState;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_StartManagementPageWrite(void){
  if(EcuabFee_Ih_WriteManagementPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK)
  {
    EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_MGMT_PAGE;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_InitiateNextWriteState(void){

  uint16 PageSize = EcuabFee_Ph_GetPartitionWriteAlignment();
  uint16 MgmtSize = (uint16) EcuabFee_MANAGEMENT_SIZE;
  uint16 PayloadLength = EcuabFee_Ih_GetInstanceDataLength(&EcuabFee_WrInst_Instance);

  if((PayloadLength <= (PageSize - MgmtSize)) || (EcuabFee_WrInst_UserJobParameter.JobType != EcuabFee_WRITE_JOB))
  {

    EcuabFee_WrInst_StartEndPatternWrite();
  }

  else if(PayloadLength > ((2u * PageSize) - MgmtSize))
  {

   if(EcuabFee_Ih_WritePayload(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK)
   {
      EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_PAYLOAD;
   }
   else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }

  else{

   if(EcuabFee_Ih_WriteLastPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK)
   {
      EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_LAST_PAGE;
   }
   else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_WriteLastPageOrEndPattern(void){

  if(EcuabFee_Ih_IsLastPageNecessary(&EcuabFee_WrInst_Instance) == TRUE)
  {

   if(EcuabFee_Ih_WriteLastPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK)
   {
      EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_LAST_PAGE;
   }
   else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

    EcuabFee_WrInst_StartEndPatternWrite();
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_StartEndPatternWrite(void){
  if(EcuabFee_Ih_WriteEndPattern(&EcuabFee_WrInst_Instance, &EcuabFee_WrInst_CompareParameter) == E_OK)
  {
    EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_END_PATTERN;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateIdle(void){

  EcuabFee_WrInst_PrepareInstanceForWriting(&EcuabFee_WrInst_Instance);

  if(EcuabFee_Ih_WriteStartPattern(&EcuabFee_WrInst_Instance, &EcuabFee_WrInst_CompareParameter) == E_OK)
  {
    EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_WRITE_START_PATTERN;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteStartPattern(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(EcuabFee_Bh_HasVerificationEnabled() == TRUE)
   {
      EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WRINST_STATE_VERIFY_START_PATTERN);
   }
   else{
      EcuabFee_WrInst_StartManagementPageWrite();
   }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteManagementPage(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(EcuabFee_Bh_HasVerificationEnabled() == TRUE)
   {
      EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WRINST_STATE_VERIFY_MGMT_PAGE);
   }
   else{
      EcuabFee_WrInst_InitiateNextWriteState();
   }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWritePayload(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(EcuabFee_Bh_HasVerificationEnabled() == TRUE)
   {
      EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WRINST_STATE_VERIFY_PAYLOAD);
   }
   else{

      EcuabFee_WrInst_WriteLastPageOrEndPattern();
   }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteLastPage(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(EcuabFee_Bh_HasVerificationEnabled() == TRUE)
   {
      EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WRINST_STATE_VERIFY_LAST_PAGE);
   }
   else{

      EcuabFee_WrInst_StartEndPatternWrite();
   }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteEndPattern(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

   if(EcuabFee_Bh_HasVerificationEnabled() == TRUE)
   {
      EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WRINST_STATE_VERIFY_END_PATTERN);
   }
   else{
      EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
   }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateMachine(void){
  switch(EcuabFee_WrInst_ComponentParameter.StateMachine)
  {

   case EcuabFee_WRINST_STATE_IDLE:
      EcuabFee_WrInst_ProcessStateIdle();
      break;

   case EcuabFee_WRINST_STATE_WRITE_START_PATTERN:
      EcuabFee_WrInst_ProcessStateWriteStartPattern();
      break;

   case EcuabFee_WRINST_STATE_VERIFY_START_PATTERN:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_StartManagementPageWrite();
      }
      else{
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

   case EcuabFee_WRINST_STATE_WRITE_MGMT_PAGE:
      EcuabFee_WrInst_ProcessStateWriteManagementPage();
      break;

   case EcuabFee_WRINST_STATE_VERIFY_MGMT_PAGE:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_InitiateNextWriteState();
      }
      else{
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

   case EcuabFee_WRINST_STATE_WRITE_PAYLOAD:
      EcuabFee_WrInst_ProcessStateWritePayload();
      break;

   case EcuabFee_WRINST_STATE_VERIFY_PAYLOAD:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_WriteLastPageOrEndPattern();
      }
      else{
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

   case EcuabFee_WRINST_STATE_WRITE_LAST_PAGE:
      EcuabFee_WrInst_ProcessStateWriteLastPage();
      break;

   case EcuabFee_WRINST_STATE_VERIFY_LAST_PAGE:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_StartEndPatternWrite();
      }
      else{
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

   case EcuabFee_WRINST_STATE_WRITE_END_PATTERN:
      EcuabFee_WrInst_ProcessStateWriteEndPattern();
      break;

   case EcuabFee_WRINST_STATE_VERIFY_END_PATTERN:
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      break;

    default:
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Init(void){
  EcuabFee_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_WrInst_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareAddress = 0x0000U;
  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareBufferPtr = NULL_PTR;
  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareLength = 0x0000U;

  EcuabFee_Ih_InitInstance(&EcuabFee_WrInst_Instance);

  EcuabFee_WrInst_ComponentParameter.StateMachine = EcuabFee_WRINST_STATE_IDLE;
}

FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_GetStatus(void){
  return EcuabFee_WrInst_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_GetJobResult(void){
  return EcuabFee_WrInst_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_StartJob(EcuabFee_Ih_InstanceType Instance
   ,     EcuabFee_UserJobParameterType UserJobParameter)
{
  Std_ReturnType retVal;

  if(EcuabFee_WrInst_ComponentParameter.StateMachine == EcuabFee_WRINST_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_WrInst_Execute, EcuabFee_WrInst_Cancel, EcuabFee_WRITE_INSTANCE_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_WrInst_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_WrInst_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_WrInst_Instance = Instance;
      EcuabFee_WrInst_UserJobParameter = UserJobParameter;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Execute(void){

  if(EcuabFee_WrInst_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_WrInst_ProcessStateMachine();
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_WrInst_Cancel(void){
  EcuabFee_WrInst_FinishJob(MEMIF_JOB_CANCELED);
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

