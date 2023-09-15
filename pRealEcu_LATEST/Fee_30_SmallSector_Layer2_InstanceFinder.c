#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE
#include "Fee_30_SmallSector_Layer2_InstanceFinder.hpp"
#include "Fee_30_SmallSector_Layer3_ReadManagementBytes.hpp"
#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"
#include "Fee_30_SmallSector_DatasetHandler.hpp"
#include "Fee_30_SmallSector_BlockHandler.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"

#define FEE_30_SMALLSECTOR_INSTANCE_FINDER_LAYER FEE_30_SMALLSECTOR_LAYER_TWO_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
  FEE_30_SMALLSECTOR_IF_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_IF_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_IF_STATE_CHECK_ERASE_PATTERN1
   ,  FEE_30_SMALLSECTOR_IF_STATE_BLANKCHECK_ERASE_PATTERN2
   ,  FEE_30_SMALLSECTOR_IF_STATE_READ_ERASE_PATTERN2
   ,  FEE_30_SMALLSECTOR_IF_STATE_EXAMINE_INSTANCE
}Fee_30_SmallSector_If_StateType;

typedef struct{
  Fee_30_SmallSector_If_ResultType JobResult;
  MemIf_StatusType Status;
  Fee_30_SmallSector_If_StateType StateMachine;
}Fee_30_SmallSector_If_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL P2VAR(Fee_30_SmallSector_Ih_InstanceType, AUTOMATIC, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_If_Instance;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Ih_InstanceType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_If_InstancePrev;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_If_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_If_ComponentParameter =
  { INSTANCE_FINDER_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_IF_STATE_UNINIT };

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_IsSearchFinished(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance
   ,     Fee_30_SmallSector_Ih_InstanceVarPointerType PreviousInstance);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_FinishJob(Fee_30_SmallSector_If_ResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessIdleState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessCheckErasePattern1(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessBlankCheckErasePattern2(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessReadErasePattern2(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessExamineInstanceState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(boolean, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_IsSearchFinished(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance
   ,     Fee_30_SmallSector_Ih_InstanceVarPointerType PreviousInstance)
{

  boolean retVal = TRUE;

  switch(Fee_30_SmallSector_Ih_GetInstanceStatus(Instance))
  {
   case INSTANCE_STATUS_ERASED:

      *Instance = *PreviousInstance;
      break;

    default :

      *PreviousInstance = *Instance;

      retVal = FALSE;
      break;
  }

  return retVal;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_FinishJob(Fee_30_SmallSector_If_ResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_If_Execute, Fee_30_SmallSector_If_Cancel, FEE_30_SMALLSECTOR_INSTANCE_FINDER_LAYER);

  Fee_30_SmallSector_If_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_If_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_IDLE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessIdleState(void){

  if(Fee_30_SmallSector_Dh_CheckErasePattern() == E_OK)
  {
    Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_CHECK_ERASE_PATTERN1;
  }
  else{
    Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessCheckErasePattern1(void){

  if((Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK) && (Fee_30_SmallSector_Dh_IsErasePatternBlank() == TRUE) )
  {

   if(Fee_30_SmallSector_Ph_GetBlankCheckApi() == TRUE)
   {

      if(Fee_30_SmallSector_Dh_BlankCheckErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_BLANKCHECK_ERASE_PATTERN2;
      }
      else{
        Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
   else{

      if(Fee_30_SmallSector_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_READ_ERASE_PATTERN2;
      }
      else{
        Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
  }
  else{

   switch(Fee_30_SmallSector_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_EP1_INCORRECT);
      break;
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessBlankCheckErasePattern2(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_BLOCK_INCONSISTENT)
  {

   if(Fee_30_SmallSector_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
   {
      Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_READ_ERASE_PATTERN2;
   }
   else{
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
   }
  }
  else{
   switch(Fee_30_SmallSector_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_EP2_INCORRECT);
      break;
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessReadErasePattern2(void){
  if((Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK) && (Fee_30_SmallSector_Dh_IsErasePatternValid() == TRUE))
  {

    Fee_30_SmallSector_Ih_CreateInstance(Fee_30_SmallSector_If_Instance, Fee_30_SmallSector_Dh_GetFirstInstanceAddress(), Fee_30_SmallSector_Dh_GetDataLength());
    Fee_30_SmallSector_If_InstancePrev = *Fee_30_SmallSector_If_Instance;

   if(Fee_30_SmallSector_RdMgmt_StartJob(Fee_30_SmallSector_If_Instance) == E_OK)
   {
      Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_EXAMINE_INSTANCE;
   }
   else{
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
   }
  }
  else{
   switch(Fee_30_SmallSector_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_EP2_INCORRECT);
      break;
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessExamineInstanceState(void){

  Fee_30_SmallSector_Ih_DetermineInstanceStatus(Fee_30_SmallSector_If_Instance);

  if(Fee_30_SmallSector_If_IsSearchFinished(Fee_30_SmallSector_If_Instance, &Fee_30_SmallSector_If_InstancePrev) == TRUE)
  {

    Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_OK);
  }
  else{

   if(Fee_30_SmallSector_Dh_IsLastInstance(Fee_30_SmallSector_If_Instance) == TRUE)
   {

      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_OK);
   }
   else{

      Fee_30_SmallSector_Dh_PrepareNextInstance(Fee_30_SmallSector_If_Instance);

      if(Fee_30_SmallSector_RdMgmt_StartJob(Fee_30_SmallSector_If_Instance) == E_OK)
      {
        Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_EXAMINE_INSTANCE;
      }
      else{
        Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_If_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_If_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_IF_STATE_IDLE:

      Fee_30_SmallSector_If_ProcessIdleState();

      break;

   case FEE_30_SMALLSECTOR_IF_STATE_CHECK_ERASE_PATTERN1:

      Fee_30_SmallSector_If_ProcessCheckErasePattern1();

      break;

   case FEE_30_SMALLSECTOR_IF_STATE_BLANKCHECK_ERASE_PATTERN2:

      Fee_30_SmallSector_If_ProcessBlankCheckErasePattern2();

      break;

   case FEE_30_SMALLSECTOR_IF_STATE_READ_ERASE_PATTERN2:

      Fee_30_SmallSector_If_ProcessReadErasePattern2();

      break;

   case FEE_30_SMALLSECTOR_IF_STATE_EXAMINE_INSTANCE:

      if(Fee_30_SmallSector_RdMgmt_GetJobResult() == MEMIF_JOB_OK)
      {

        Fee_30_SmallSector_If_ProcessExamineInstanceState();
      }
      else{

        Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
      break;

    default:
      Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Init(void){
  Fee_30_SmallSector_If_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_If_ComponentParameter.JobResult = INSTANCE_FINDER_OK;

  Fee_30_SmallSector_Ih_InitInstance(&Fee_30_SmallSector_If_InstancePrev);

  Fee_30_SmallSector_If_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_IF_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_GetStatus(void){
  return Fee_30_SmallSector_If_ComponentParameter.Status;
}

FUNC(Fee_30_SmallSector_If_ResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_GetJobResult(void){
  return Fee_30_SmallSector_If_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_StartJob(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_If_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_IF_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_If_Execute, Fee_30_SmallSector_If_Cancel, FEE_30_SMALLSECTOR_INSTANCE_FINDER_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_If_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_If_ComponentParameter.JobResult = INSTANCE_FINDER_PENDING;

      Fee_30_SmallSector_If_Instance = Instance;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Execute(void){

  if(Fee_30_SmallSector_If_ComponentParameter.Status == MEMIF_BUSY)
  {
    Fee_30_SmallSector_If_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Cancel(void){
  Fee_30_SmallSector_If_FinishJob(INSTANCE_FINDER_CANCELED);
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_MapResult(Fee_30_SmallSector_If_ResultType JobResult)
{
  MemIf_JobResultType retVal;

  switch(JobResult)
  {
  case INSTANCE_FINDER_OK:
    retVal = MEMIF_JOB_OK;
   break;
  case INSTANCE_FINDER_FAILED:
    retVal = MEMIF_JOB_FAILED;
   break;
  case INSTANCE_FINDER_PENDING:
    retVal = MEMIF_JOB_PENDING;
   break;
  case INSTANCE_FINDER_CANCELED:
    retVal = MEMIF_JOB_CANCELED;
   break;
  default:
    retVal = MEMIF_BLOCK_INCONSISTENT;
   break;
  }

  return retVal;
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

