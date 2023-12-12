#include "Std_Types.hpp"

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer3_ReadManagementBytes.hpp"

#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#include "EcuabFee_PartitionHandler.hpp"

#define EcuabFee_READ_MANAGEMENT_BYTES_LAYER EcuabFee_LAYER_THREE_INDEX

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef enum{
  EcuabFee_RDMGMT_STATE_UNINIT = 0
   ,  EcuabFee_RDMGMT_STATE_IDLE
   ,  EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START
   ,  EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END
   ,  EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST
   ,  EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_LAST
   ,  EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_ALL
}EcuabFee_RdMgmt_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_RdMgmt_StateType StateMachine;
}EcuabFee_RdMgmt_ComponentParameterType;

#define EcuabFee_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_RdMgmt_ComponentParameterType, EcuabFee_APPL_DATA) EcuabFee_RdMgmt_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, EcuabFee_RDMGMT_STATE_UNINIT };

#define EcuabFee_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL P2VAR(EcuabFee_Ih_InstanceType, AUTOMATIC, EcuabFee_APPL_DATA) EcuabFee_RdMgmt_Instance;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_FinishJob(MemIf_JobResultType JobResult);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessIdleState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForStartBlankCheckState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForEndBlankCheckState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForLastBlankCheckState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForReadLastPage(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForReadAllState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessStateMachine(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_RdMgmt_Execute, EcuabFee_RdMgmt_Cancel, EcuabFee_READ_MANAGEMENT_BYTES_LAYER);

  EcuabFee_RdMgmt_ComponentParameter.JobResult = JobResult;
  EcuabFee_RdMgmt_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_IDLE;
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessIdleState(void){

  EcuabFee_Ih_SetWP1State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INIT);
  EcuabFee_Ih_SetWP2State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INIT);
  EcuabFee_Ih_SetLastPageState(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INIT);

  if(EcuabFee_Ph_GetBlankCheckApi() == TRUE)
  {

   if(EcuabFee_Ih_BlankCheckStartPattern(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

   if(EcuabFee_Ih_ReadLastPage(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_LAST;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForStartBlankCheckState(void){
  if(EcuabFee_Fls_GetJobResult() != MEMIF_JOB_FAILED){

   if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
   {

      EcuabFee_Ih_SetWP1State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_BLANK);
   }
   else{

      EcuabFee_Ih_SetWP1State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INCONSISTENT);
   }

   if(EcuabFee_Ih_BlankCheckEndPattern(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{
    EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForEndBlankCheckState(void){
  if(EcuabFee_Fls_GetJobResult() != MEMIF_JOB_FAILED){

   if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
   {
      EcuabFee_Ih_SetWP2State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_BLANK);
   }
   else{

      EcuabFee_Ih_SetWP2State(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INCONSISTENT);
   }

   if(EcuabFee_Ih_BlankCheckLastPage(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{
    EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForLastBlankCheckState(void){
  MemIf_JobResultType currentJobResult = EcuabFee_Fls_GetJobResult();
  boolean IsWp1Inconsistent = (boolean) (EcuabFee_Ih_GetWP1State(EcuabFee_RdMgmt_Instance) == EcuabFee_PATTERN_INCONSISTENT);
  boolean IsWp2Inconsistent = (boolean) (EcuabFee_Ih_GetWP2State(EcuabFee_RdMgmt_Instance) == EcuabFee_PATTERN_INCONSISTENT);

  switch(currentJobResult)
  {
  case MEMIF_JOB_OK:

    EcuabFee_Ih_SetLastPageState(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_BLANK);
   break;

  case MEMIF_BLOCK_INCONSISTENT:

    EcuabFee_Ih_SetLastPageState(EcuabFee_RdMgmt_Instance, EcuabFee_PATTERN_INCONSISTENT);
   break;

  default:

    EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   break;
  }

  if(IsWp1Inconsistent || IsWp2Inconsistent)
  {

   if(EcuabFee_Ih_ReadAllMgmt(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_ALL;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

    EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_OK);
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForReadLastPage(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){

    EcuabFee_Ih_SetLastPageState(EcuabFee_RdMgmt_Instance, EcuabFee_Ih_GetPageStatusFromBuffer());

   if(EcuabFee_Ih_ReadAllMgmt(EcuabFee_RdMgmt_Instance) == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_ALL;
   }
   else{
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }

  }
  else{
    EcuabFee_RdMgmt_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessWaitForReadAllState(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){

    EcuabFee_Ih_SetWP1State(EcuabFee_RdMgmt_Instance, EcuabFee_Ih_GetStartPatternFromBuffer(EcuabFee_RdMgmt_Instance));

    EcuabFee_Ih_SetWP2State(EcuabFee_RdMgmt_Instance, EcuabFee_Ih_GetEndPatternFromBuffer(EcuabFee_RdMgmt_Instance));

    EcuabFee_Ih_StoreMgmtValueFromBuffer(EcuabFee_RdMgmt_Instance);

#if(EcuabFee_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
    EcuabFee_Ih_CorrectSingleBitFlips(&(EcuabFee_RdMgmt_Instance)->MgmtValue);
#   endif

    EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_OK);
  }
  else{
    EcuabFee_RdMgmt_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_RdMgmt_ProcessStateMachine(void){
  switch(EcuabFee_RdMgmt_ComponentParameter.StateMachine)
  {

   case EcuabFee_RDMGMT_STATE_IDLE:

      EcuabFee_RdMgmt_ProcessIdleState();
      break;

   case EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START:

      EcuabFee_RdMgmt_ProcessWaitForStartBlankCheckState();
      break;

   case EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END:

      EcuabFee_RdMgmt_ProcessWaitForEndBlankCheckState();
      break;

   case EcuabFee_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST:

      EcuabFee_RdMgmt_ProcessWaitForLastBlankCheckState();
      break;

   case EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_LAST:

      EcuabFee_RdMgmt_ProcessWaitForReadLastPage();
      break;

   case EcuabFee_RDMGMT_STATE_WAIT_FOR_READ_ALL:

      EcuabFee_RdMgmt_ProcessWaitForReadAllState();
      break;

    default:
      EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Init(void){
  EcuabFee_RdMgmt_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_RdMgmt_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_RdMgmt_ComponentParameter.StateMachine = EcuabFee_RDMGMT_STATE_IDLE;
}

FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_GetStatus(void){
  return EcuabFee_RdMgmt_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_GetJobResult(void){
  return EcuabFee_RdMgmt_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_StartJob(EcuabFee_Ih_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  if(EcuabFee_RdMgmt_ComponentParameter.StateMachine == EcuabFee_RDMGMT_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_RdMgmt_Execute, EcuabFee_RdMgmt_Cancel, EcuabFee_READ_MANAGEMENT_BYTES_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_RdMgmt_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_RdMgmt_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_RdMgmt_Instance = Instance;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Execute(void){

  if(EcuabFee_RdMgmt_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_RdMgmt_ProcessStateMachine();
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_RdMgmt_Cancel(void){
  EcuabFee_RdMgmt_FinishJob(MEMIF_JOB_CANCELED);
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

