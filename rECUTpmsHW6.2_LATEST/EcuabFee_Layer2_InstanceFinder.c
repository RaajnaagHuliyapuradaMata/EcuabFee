#include "Std_Types.hpp"

#define EcuabFee_IMPLEMENTATION_SOURCE
#include "EcuabFee_Layer2_InstanceFinder.hpp"
#include "EcuabFee_Layer3_ReadManagementBytes.hpp"
#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_BlockHandler.hpp"
#include "EcuabFee_PartitionHandler.hpp"

#define EcuabFee_INSTANCE_FINDER_LAYER EcuabFee_LAYER_TWO_INDEX

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef enum{
  EcuabFee_IF_STATE_UNINIT = 0
   ,  EcuabFee_IF_STATE_IDLE
   ,  EcuabFee_IF_STATE_CHECK_ERASE_PATTERN1
   ,  EcuabFee_IF_STATE_BLANKCHECK_ERASE_PATTERN2
   ,  EcuabFee_IF_STATE_READ_ERASE_PATTERN2
   ,  EcuabFee_IF_STATE_EXAMINE_INSTANCE
}EcuabFee_If_StateType;

typedef struct{
  EcuabFee_If_ResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_If_StateType StateMachine;
}EcuabFee_If_ComponentParameterType;

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL P2VAR(EcuabFee_Ih_InstanceType, AUTOMATIC, ECUABFEE_APPL_DATA) EcuabFee_If_Instance;

EcuabFee_LOCAL VAR(EcuabFee_Ih_InstanceType, ECUABFEE_APPL_DATA) EcuabFee_If_InstancePrev;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_If_ComponentParameterType, ECUABFEE_APPL_DATA) EcuabFee_If_ComponentParameter =
  { INSTANCE_FINDER_FAILED, MEMIF_UNINIT, EcuabFee_IF_STATE_UNINIT };

#define EcuabFee_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(boolean, ECUABFEE_PRIVATE_CODE) EcuabFee_If_IsSearchFinished(EcuabFee_Ih_InstanceVarPointerType Instance
   ,     EcuabFee_Ih_InstanceVarPointerType PreviousInstance);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_FinishJob(EcuabFee_If_ResultType JobResult);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessIdleState(void);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessCheckErasePattern1(void);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessBlankCheckErasePattern2(void);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessReadErasePattern2(void);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessExamineInstanceState(void);

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessStateMachine(void);

EcuabFee_LOCAL FUNC(boolean, ECUABFEE_PRIVATE_CODE) EcuabFee_If_IsSearchFinished(EcuabFee_Ih_InstanceVarPointerType Instance
   ,     EcuabFee_Ih_InstanceVarPointerType PreviousInstance)
{

  boolean retVal = TRUE;

  switch(EcuabFee_Ih_GetInstanceStatus(Instance))
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

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_FinishJob(EcuabFee_If_ResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_If_Execute, EcuabFee_If_Cancel, EcuabFee_INSTANCE_FINDER_LAYER);

  EcuabFee_If_ComponentParameter.JobResult = JobResult;
  EcuabFee_If_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_IDLE;
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessIdleState(void){

  if(EcuabFee_Dh_CheckErasePattern() == E_OK)
  {
    EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_CHECK_ERASE_PATTERN1;
  }
  else{
    EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
  }
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessCheckErasePattern1(void){

  if((EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK) && (EcuabFee_Dh_IsErasePatternBlank() == TRUE) )
  {

   if(EcuabFee_Ph_GetBlankCheckApi() == TRUE)
   {

      if(EcuabFee_Dh_BlankCheckErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_BLANKCHECK_ERASE_PATTERN2;
      }
      else{
        EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
   else{

      if(EcuabFee_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_READ_ERASE_PATTERN2;
      }
      else{
        EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
  }
  else{

   switch(EcuabFee_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_EP1_INCORRECT);
      break;
   }
  }
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessBlankCheckErasePattern2(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_BLOCK_INCONSISTENT)
  {

   if(EcuabFee_Dh_ReadErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
   {
      EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_READ_ERASE_PATTERN2;
   }
   else{
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
   }
  }
  else{
   switch(EcuabFee_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_EP2_INCORRECT);
      break;
   }
  }
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessReadErasePattern2(void){
  if((EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK) && (EcuabFee_Dh_IsErasePatternValid() == TRUE))
  {

    EcuabFee_Ih_CreateInstance(EcuabFee_If_Instance, EcuabFee_Dh_GetFirstInstanceAddress(), EcuabFee_Dh_GetDataLength());
    EcuabFee_If_InstancePrev = *EcuabFee_If_Instance;

   if(EcuabFee_RdMgmt_StartJob(EcuabFee_If_Instance) == E_OK)
   {
      EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_EXAMINE_INSTANCE;
   }
   else{
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
   }
  }
  else{
   switch(EcuabFee_Fls_GetJobResult())
   {
   case MEMIF_JOB_FAILED:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
    default:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_EP2_INCORRECT);
      break;
   }
  }
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessExamineInstanceState(void){

  EcuabFee_Ih_DetermineInstanceStatus(EcuabFee_If_Instance);

  if(EcuabFee_If_IsSearchFinished(EcuabFee_If_Instance, &EcuabFee_If_InstancePrev) == TRUE)
  {

    EcuabFee_If_FinishJob(INSTANCE_FINDER_OK);
  }
  else{

   if(EcuabFee_Dh_IsLastInstance(EcuabFee_If_Instance) == TRUE)
   {

      EcuabFee_If_FinishJob(INSTANCE_FINDER_OK);
   }
   else{

      EcuabFee_Dh_PrepareNextInstance(EcuabFee_If_Instance);

      if(EcuabFee_RdMgmt_StartJob(EcuabFee_If_Instance) == E_OK)
      {
        EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_EXAMINE_INSTANCE;
      }
      else{
        EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
   }
  }
}

EcuabFee_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_If_ProcessStateMachine(void){
  switch(EcuabFee_If_ComponentParameter.StateMachine)
  {

   case EcuabFee_IF_STATE_IDLE:

      EcuabFee_If_ProcessIdleState();

      break;

   case EcuabFee_IF_STATE_CHECK_ERASE_PATTERN1:

      EcuabFee_If_ProcessCheckErasePattern1();

      break;

   case EcuabFee_IF_STATE_BLANKCHECK_ERASE_PATTERN2:

      EcuabFee_If_ProcessBlankCheckErasePattern2();

      break;

   case EcuabFee_IF_STATE_READ_ERASE_PATTERN2:

      EcuabFee_If_ProcessReadErasePattern2();

      break;

   case EcuabFee_IF_STATE_EXAMINE_INSTANCE:

      if(EcuabFee_RdMgmt_GetJobResult() == MEMIF_JOB_OK)
      {

        EcuabFee_If_ProcessExamineInstanceState();
      }
      else{

        EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      }
      break;

    default:
      EcuabFee_If_FinishJob(INSTANCE_FINDER_FAILED);
      break;
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Init(void){
  EcuabFee_If_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_If_ComponentParameter.JobResult = INSTANCE_FINDER_OK;

  EcuabFee_Ih_InitInstance(&EcuabFee_If_InstancePrev);

  EcuabFee_If_ComponentParameter.StateMachine = EcuabFee_IF_STATE_IDLE;
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_GetStatus(void){
  return EcuabFee_If_ComponentParameter.Status;
}

FUNC(EcuabFee_If_ResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_GetJobResult(void){
  return EcuabFee_If_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_StartJob(EcuabFee_Ih_InstanceVarPointerType Instance)
{
  Std_ReturnType retVal;

  if(EcuabFee_If_ComponentParameter.StateMachine == EcuabFee_IF_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_If_Execute, EcuabFee_If_Cancel, EcuabFee_INSTANCE_FINDER_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_If_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_If_ComponentParameter.JobResult = INSTANCE_FINDER_PENDING;

      EcuabFee_If_Instance = Instance;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Execute(void){

  if(EcuabFee_If_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_If_ProcessStateMachine();
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_If_Cancel(void){
  EcuabFee_If_FinishJob(INSTANCE_FINDER_CANCELED);
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_If_MapResult(EcuabFee_If_ResultType JobResult)
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

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

