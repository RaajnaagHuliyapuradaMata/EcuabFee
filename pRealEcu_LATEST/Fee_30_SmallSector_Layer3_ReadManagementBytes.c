#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_Layer3_ReadManagementBytes.hpp"

#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"

#include "Fee_30_SmallSector_PartitionHandler.hpp"

#define FEE_30_SMALLSECTOR_READ_MANAGEMENT_BYTES_LAYER FEE_30_SMALLSECTOR_LAYER_THREE_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
  FEE_30_SMALLSECTOR_RDMGMT_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_LAST
   ,  FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_ALL
}Fee_30_SmallSector_RdMgmt_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  Fee_30_SmallSector_RdMgmt_StateType StateMachine;
}Fee_30_SmallSector_RdMgmt_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_RdMgmt_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_RdMgmt_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_RDMGMT_STATE_UNINIT };

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL P2VAR(Fee_30_SmallSector_Ih_InstanceType, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_RdMgmt_Instance;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessIdleState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForStartBlankCheckState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForEndBlankCheckState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForLastBlankCheckState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForReadLastPage(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForReadAllState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_RdMgmt_Execute, Fee_30_SmallSector_RdMgmt_Cancel, FEE_30_SMALLSECTOR_READ_MANAGEMENT_BYTES_LAYER);

  Fee_30_SmallSector_RdMgmt_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_RdMgmt_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_IDLE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessIdleState(void){

  Fee_30_SmallSector_Ih_SetWP1State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INIT);
  Fee_30_SmallSector_Ih_SetWP2State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INIT);
  Fee_30_SmallSector_Ih_SetLastPageState(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INIT);

  if(Fee_30_SmallSector_Ph_GetBlankCheckApi() == TRUE)
  {

   if(Fee_30_SmallSector_Ih_BlankCheckStartPattern(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

   if(Fee_30_SmallSector_Ih_ReadLastPage(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_LAST;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForStartBlankCheckState(void){
  if(Fee_30_SmallSector_Fls_GetJobResult() != MEMIF_JOB_FAILED){

   if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
   {

      Fee_30_SmallSector_Ih_SetWP1State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_BLANK);
   }
   else{

      Fee_30_SmallSector_Ih_SetWP1State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT);
   }

   if(Fee_30_SmallSector_Ih_BlankCheckEndPattern(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{
    Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForEndBlankCheckState(void){
  if(Fee_30_SmallSector_Fls_GetJobResult() != MEMIF_JOB_FAILED){

   if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
   {
      Fee_30_SmallSector_Ih_SetWP2State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_BLANK);
   }
   else{

      Fee_30_SmallSector_Ih_SetWP2State(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT);
   }

   if(Fee_30_SmallSector_Ih_BlankCheckLastPage(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{
    Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForLastBlankCheckState(void){
  MemIf_JobResultType currentJobResult = Fee_30_SmallSector_Fls_GetJobResult();
  boolean IsWp1Inconsistent = (boolean) (Fee_30_SmallSector_Ih_GetWP1State(Fee_30_SmallSector_RdMgmt_Instance) == FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT);
  boolean IsWp2Inconsistent = (boolean) (Fee_30_SmallSector_Ih_GetWP2State(Fee_30_SmallSector_RdMgmt_Instance) == FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT);

  switch(currentJobResult)
  {
  case MEMIF_JOB_OK:

    Fee_30_SmallSector_Ih_SetLastPageState(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_BLANK);
   break;

  case MEMIF_BLOCK_INCONSISTENT:

    Fee_30_SmallSector_Ih_SetLastPageState(Fee_30_SmallSector_RdMgmt_Instance, FEE_30_SMALLSECTOR_PATTERN_INCONSISTENT);
   break;

  default:

    Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   break;
  }

  if(IsWp1Inconsistent || IsWp2Inconsistent)
  {

   if(Fee_30_SmallSector_Ih_ReadAllMgmt(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_ALL;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }
  }
  else{

    Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_OK);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForReadLastPage(void){
  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK){

    Fee_30_SmallSector_Ih_SetLastPageState(Fee_30_SmallSector_RdMgmt_Instance, Fee_30_SmallSector_Ih_GetPageStatusFromBuffer());

   if(Fee_30_SmallSector_Ih_ReadAllMgmt(Fee_30_SmallSector_RdMgmt_Instance) == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_ALL;
   }
   else{
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
   }

  }
  else{
    Fee_30_SmallSector_RdMgmt_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessWaitForReadAllState(void){
  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK){

    Fee_30_SmallSector_Ih_SetWP1State(Fee_30_SmallSector_RdMgmt_Instance, Fee_30_SmallSector_Ih_GetStartPatternFromBuffer(Fee_30_SmallSector_RdMgmt_Instance));

    Fee_30_SmallSector_Ih_SetWP2State(Fee_30_SmallSector_RdMgmt_Instance, Fee_30_SmallSector_Ih_GetEndPatternFromBuffer(Fee_30_SmallSector_RdMgmt_Instance));

    Fee_30_SmallSector_Ih_StoreMgmtValueFromBuffer(Fee_30_SmallSector_RdMgmt_Instance);

#if(FEE_30_SMALLSECTOR_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
    Fee_30_SmallSector_Ih_CorrectSingleBitFlips(&(Fee_30_SmallSector_RdMgmt_Instance)->MgmtValue);
#   endif

    Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_OK);
  }
  else{
    Fee_30_SmallSector_RdMgmt_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_RdMgmt_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_IDLE:

      Fee_30_SmallSector_RdMgmt_ProcessIdleState();
      break;

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_START:

      Fee_30_SmallSector_RdMgmt_ProcessWaitForStartBlankCheckState();
      break;

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_END:

      Fee_30_SmallSector_RdMgmt_ProcessWaitForEndBlankCheckState();
      break;

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_BLANKCHECK_LAST:

      Fee_30_SmallSector_RdMgmt_ProcessWaitForLastBlankCheckState();
      break;

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_LAST:

      Fee_30_SmallSector_RdMgmt_ProcessWaitForReadLastPage();
      break;

   case FEE_30_SMALLSECTOR_RDMGMT_STATE_WAIT_FOR_READ_ALL:

      Fee_30_SmallSector_RdMgmt_ProcessWaitForReadAllState();
      break;

    default:
      Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_Init(void){
  Fee_30_SmallSector_RdMgmt_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_RdMgmt_ComponentParameter.JobResult = MEMIF_JOB_OK;

  Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RDMGMT_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_GetStatus(void){
  return Fee_30_SmallSector_RdMgmt_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_GetJobResult(void){
  return Fee_30_SmallSector_RdMgmt_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_StartJob(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_RdMgmt_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_RDMGMT_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_RdMgmt_Execute, Fee_30_SmallSector_RdMgmt_Cancel, FEE_30_SMALLSECTOR_READ_MANAGEMENT_BYTES_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_RdMgmt_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_RdMgmt_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      Fee_30_SmallSector_RdMgmt_Instance = Instance;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_Execute(void){

  if(Fee_30_SmallSector_RdMgmt_ComponentParameter.Status == MEMIF_BUSY)
  {
    Fee_30_SmallSector_RdMgmt_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_RdMgmt_Cancel(void){
  Fee_30_SmallSector_RdMgmt_FinishJob(MEMIF_JOB_CANCELED);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

