

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_Layer1_Write.hpp"
#include "Fee_30_SmallSector_Layer2_InstanceFinder.hpp"
#include "Fee_30_SmallSector_Layer2_DatasetEraser.hpp"
#include "Fee_30_SmallSector_Layer2_WriteInstance.hpp"

#include "Fee_30_SmallSector_DatasetHandler.hpp"
#include "Fee_30_SmallSector_InstanceHandler.hpp"
#include "Fee_30_SmallSector_TaskManager.hpp"

#define FEE_30_SMALLSECTOR_WRITE_LAYER FEE_30_SMALLSECTOR_LAYER_ONE_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
  FEE_30_SMALLSECTOR_WR_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_WR_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_WR_STATE_WAIT_FOR_INSTANCEFINDER
   ,  FEE_30_SMALLSECTOR_WR_STATE_ERASE_DATASET
   ,  FEE_30_SMALLSECTOR_WR_STATE_WRITE_INSTANCE
}Fee_30_SmallSector_Wr_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  Fee_30_SmallSector_Wr_StateType StateMachine;
  boolean SuspendWrites;
}Fee_30_SmallSector_Wr_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Wr_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Wr_ComponentParameter =
   {MEMIF_JOB_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_WR_STATE_UNINIT, FALSE};

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Ih_InstanceType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Wr_Instance;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_UserJobParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Wr_UserJobParameter;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_InitiateDatasetErase(Fee_30_SmallSector_If_ResultType InstanceFinderResult);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_InitiateWriteInstance(void);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessInstanceFinderState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessDatasetEraseState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_InitiateDatasetErase(Fee_30_SmallSector_If_ResultType InstanceFinderResult)
{

  if(Fee_30_SmallSector_De_StartJob(InstanceFinderResult) == E_OK)
  {
    Fee_30_SmallSector_Wr_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WR_STATE_ERASE_DATASET;
  }
  else{
    Fee_30_SmallSector_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_InitiateWriteInstance(void){

  if(Fee_30_SmallSector_WrInst_StartJob(Fee_30_SmallSector_Wr_Instance, Fee_30_SmallSector_Wr_UserJobParameter) == E_OK)
  {
    Fee_30_SmallSector_Wr_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WR_STATE_WRITE_INSTANCE;
  }
  else{
    Fee_30_SmallSector_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessInstanceFinderState(void){
  Fee_30_SmallSector_If_ResultType InstanceFinderResult = Fee_30_SmallSector_If_GetJobResult();

  switch(InstanceFinderResult)
  {
  case INSTANCE_FINDER_OK:
  {
   boolean IsLastInstance = Fee_30_SmallSector_Dh_IsLastInstance(&Fee_30_SmallSector_Wr_Instance) == TRUE;

   if((Fee_30_SmallSector_Dh_IsFirstInstanceAndBlank(&Fee_30_SmallSector_Wr_Instance) == TRUE) || (IsLastInstance == TRUE))
   {

      Fee_30_SmallSector_Wr_InitiateDatasetErase(InstanceFinderResult);
   }
   else{

      Fee_30_SmallSector_Dh_PrepareNextInstance(&Fee_30_SmallSector_Wr_Instance);

      Fee_30_SmallSector_Wr_InitiateWriteInstance();
   }
   break;
  }
  case INSTANCE_FINDER_EP1_INCORRECT:
  case INSTANCE_FINDER_EP2_INCORRECT:

    Fee_30_SmallSector_Wr_InitiateDatasetErase(InstanceFinderResult);
   break;

  default:
    Fee_30_SmallSector_Wr_FinishJob(Fee_30_SmallSector_If_MapResult(InstanceFinderResult));
   break;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessDatasetEraseState(void){

  if(Fee_30_SmallSector_De_GetJobResult() == MEMIF_JOB_OK)
  {

    Fee_30_SmallSector_Ih_CreateInstance(&Fee_30_SmallSector_Wr_Instance, Fee_30_SmallSector_Dh_GetFirstInstanceAddress(), Fee_30_SmallSector_Dh_GetDataLength());
    Fee_30_SmallSector_Wr_InitiateWriteInstance();
  }
  else{
    Fee_30_SmallSector_Wr_FinishJob(Fee_30_SmallSector_De_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_Wr_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_WR_STATE_IDLE:

      if(Fee_30_SmallSector_If_StartJob(&Fee_30_SmallSector_Wr_Instance) == E_OK)
      {
        Fee_30_SmallSector_Wr_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WR_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else{
        Fee_30_SmallSector_Wr_FinishJob(MEMIF_JOB_FAILED);
      }

      break;

   case FEE_30_SMALLSECTOR_WR_STATE_WAIT_FOR_INSTANCEFINDER:

      Fee_30_SmallSector_Wr_ProcessInstanceFinderState();

      break;

   case FEE_30_SMALLSECTOR_WR_STATE_ERASE_DATASET:

      Fee_30_SmallSector_Wr_ProcessDatasetEraseState();

      break;

   case FEE_30_SMALLSECTOR_WR_STATE_WRITE_INSTANCE:

      Fee_30_SmallSector_Wr_FinishJob(Fee_30_SmallSector_WrInst_GetJobResult());

      break;

    default:
      Fee_30_SmallSector_Wr_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC (void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Wr_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_Wr_Execute, Fee_30_SmallSector_Wr_Cancel, FEE_30_SMALLSECTOR_WRITE_LAYER);

  Fee_30_SmallSector_Wr_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_Wr_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WR_STATE_IDLE;
  Fee_30_SmallSector_Wr_ComponentParameter.Status = MEMIF_IDLE;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_Init(void){
  Fee_30_SmallSector_Wr_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_Wr_ComponentParameter.JobResult = MEMIF_JOB_OK;

  Fee_30_SmallSector_Ih_InitInstance(&Fee_30_SmallSector_Wr_Instance);

  Fee_30_SmallSector_Wr_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_WR_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_GetStatus(void){
  return Fee_30_SmallSector_Wr_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_GetJobResult(void){
  return Fee_30_SmallSector_Wr_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_StartJob(Fee_30_SmallSector_UserJobParameterType Fee_30_SmallSector_UserJobParameter)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_Wr_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_WR_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_Wr_Execute, Fee_30_SmallSector_Wr_Cancel, FEE_30_SMALLSECTOR_WRITE_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_Wr_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_Wr_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      Fee_30_SmallSector_Wr_UserJobParameter = Fee_30_SmallSector_UserJobParameter;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_Execute(void){

  if((Fee_30_SmallSector_Wr_ComponentParameter.Status == MEMIF_BUSY) &&
      (Fee_30_SmallSector_Wr_ComponentParameter.SuspendWrites == FALSE))
  {
      Fee_30_SmallSector_Wr_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_Cancel(void){
  Fee_30_SmallSector_Wr_FinishJob(MEMIF_JOB_CANCELED);
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_SuspendWrites(void){

  Fee_30_SmallSector_Wr_ComponentParameter.SuspendWrites = TRUE;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Wr_ResumeWrites(void){

  Fee_30_SmallSector_Wr_ComponentParameter.SuspendWrites = FALSE;
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

