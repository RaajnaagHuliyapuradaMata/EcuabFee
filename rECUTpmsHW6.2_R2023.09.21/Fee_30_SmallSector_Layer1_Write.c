#include "Std_Types.hpp"

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer1_Write.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"
#include "EcuabFee_Layer2_DatasetEraser.hpp"
#include "EcuabFee_Layer2_WriteInstance.hpp"
#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_InstanceHandler.hpp"
#include "EcuabFee_TaskManager.hpp"

#define EcuabFee_WRITE_LAYER EcuabFee_LAYER_ONE_INDEX

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef enum{
  EcuabFee_WR_STATE_UNINIT = 0
   ,  EcuabFee_WR_STATE_IDLE
   ,  EcuabFee_WR_STATE_WAIT_FOR_INSTANCEFINDER
   ,  EcuabFee_WR_STATE_ERASE_DATASET
   ,  EcuabFee_WR_STATE_WRITE_INSTANCE
}EcuabFee_Wr_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_Wr_StateType StateMachine;
  boolean SuspendWrites;
}EcuabFee_Wr_ComponentParameterType;

#define EcuabFee_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Wr_ComponentParameterType, EcuabFee_APPL_DATA) EcuabFee_Wr_ComponentParameter =
   {MEMIF_JOB_FAILED, MEMIF_UNINIT, EcuabFee_WR_STATE_UNINIT, FALSE};

#define EcuabFee_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Ih_InstanceType, EcuabFee_APPL_DATA) EcuabFee_Wr_Instance;

EcuabFee_LOCAL VAR(EcuabFee_UserJobParameterType, EcuabFee_APPL_DATA) EcuabFee_Wr_UserJobParameter;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_InitiateDatasetErase(EcuabFee_If_ResultType InstanceFinderResult);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_InitiateWriteInstance(void);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessInstanceFinderState(void);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessDatasetEraseState(void);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessStateMachine(void);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_FinishJob(MemIf_JobResultType JobResult);

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_InitiateDatasetErase(EcuabFee_If_ResultType InstanceFinderResult)
{

  if(EcuabFee_De_StartJob(InstanceFinderResult) == E_OK)
  {
    EcuabFee_Wr_ComponentParameter.StateMachine = EcuabFee_WR_STATE_ERASE_DATASET;
  }
  else{
    EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_InitiateWriteInstance(void){

  if(EcuabFee_WrInst_StartJob(EcuabFee_Wr_Instance, EcuabFee_Wr_UserJobParameter) == E_OK)
  {
    EcuabFee_Wr_ComponentParameter.StateMachine = EcuabFee_WR_STATE_WRITE_INSTANCE;
  }
  else{
    EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessInstanceFinderState(void){
  EcuabFee_If_ResultType InstanceFinderResult = EcuabFee_If_GetJobResult();

  switch(InstanceFinderResult)
  {
  case INSTANCE_FINDER_OK:
  {
   boolean IsLastInstance = EcuabFee_Dh_IsLastInstance(&EcuabFee_Wr_Instance) == TRUE;

   if((EcuabFee_Dh_IsFirstInstanceAndBlank(&EcuabFee_Wr_Instance) == TRUE) || (IsLastInstance == TRUE))
   {

      EcuabFee_Wr_InitiateDatasetErase(InstanceFinderResult);
   }
   else{

      EcuabFee_Dh_PrepareNextInstance(&EcuabFee_Wr_Instance);

      EcuabFee_Wr_InitiateWriteInstance();
   }
   break;
  }
  case INSTANCE_FINDER_EP1_INCORRECT:
  case INSTANCE_FINDER_EP2_INCORRECT:

    EcuabFee_Wr_InitiateDatasetErase(InstanceFinderResult);
   break;

  default:
    EcuabFee_Wr_FinishJob(EcuabFee_If_MapResult(InstanceFinderResult));
   break;
  }
}

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessDatasetEraseState(void){

  if(EcuabFee_De_GetJobResult() == MEMIF_JOB_OK)
  {

    EcuabFee_Ih_CreateInstance(&EcuabFee_Wr_Instance, EcuabFee_Dh_GetFirstInstanceAddress(), EcuabFee_Dh_GetDataLength());
    EcuabFee_Wr_InitiateWriteInstance();
  }
  else{
    EcuabFee_Wr_FinishJob(EcuabFee_De_GetJobResult());
  }
}

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_ProcessStateMachine(void){
  switch(EcuabFee_Wr_ComponentParameter.StateMachine)
  {

   case EcuabFee_WR_STATE_IDLE:

      if(EcuabFee_If_StartJob(&EcuabFee_Wr_Instance) == E_OK)
      {
        EcuabFee_Wr_ComponentParameter.StateMachine = EcuabFee_WR_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else{
        EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
      }

      break;

   case EcuabFee_WR_STATE_WAIT_FOR_INSTANCEFINDER:

      EcuabFee_Wr_ProcessInstanceFinderState();

      break;

   case EcuabFee_WR_STATE_ERASE_DATASET:

      EcuabFee_Wr_ProcessDatasetEraseState();

      break;

   case EcuabFee_WR_STATE_WRITE_INSTANCE:

      EcuabFee_Wr_FinishJob(EcuabFee_WrInst_GetJobResult());

      break;

    default:
      EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

EcuabFee_LOCAL FUNC (void, EcuabFee_PRIVATE_CODE) EcuabFee_Wr_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_Wr_Execute, EcuabFee_Wr_Cancel, EcuabFee_WRITE_LAYER);

  EcuabFee_Wr_ComponentParameter.JobResult = JobResult;
  EcuabFee_Wr_ComponentParameter.StateMachine = EcuabFee_WR_STATE_IDLE;
  EcuabFee_Wr_ComponentParameter.Status = MEMIF_IDLE;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Init(void){
  EcuabFee_Wr_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Wr_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_Ih_InitInstance(&EcuabFee_Wr_Instance);

  EcuabFee_Wr_ComponentParameter.StateMachine = EcuabFee_WR_STATE_IDLE;
}

FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_GetStatus(void){
  return EcuabFee_Wr_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_GetJobResult(void){
  return EcuabFee_Wr_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter)
{
  Std_ReturnType retVal;

  if(EcuabFee_Wr_ComponentParameter.StateMachine == EcuabFee_WR_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_Wr_Execute, EcuabFee_Wr_Cancel, EcuabFee_WRITE_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_Wr_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_Wr_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_Wr_UserJobParameter = EcuabFee_UserJobParameter;
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Execute(void){

  if((EcuabFee_Wr_ComponentParameter.Status == MEMIF_BUSY) &&
      (EcuabFee_Wr_ComponentParameter.SuspendWrites == FALSE))
  {
      EcuabFee_Wr_ProcessStateMachine();
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_Cancel(void){
  EcuabFee_Wr_FinishJob(MEMIF_JOB_CANCELED);
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_SuspendWrites(void){

  EcuabFee_Wr_ComponentParameter.SuspendWrites = TRUE;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Wr_ResumeWrites(void){

  EcuabFee_Wr_ComponentParameter.SuspendWrites = FALSE;
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

