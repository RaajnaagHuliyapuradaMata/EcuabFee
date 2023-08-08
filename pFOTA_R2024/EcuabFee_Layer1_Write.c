

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer1_Write.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"
#include "EcuabFee_Layer2_DatasetEraser.hpp"
#include "EcuabFee_Layer2_WriteInstance.hpp"

#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_InstanceHandler.hpp"
#include "EcuabFee_TaskManager.hpp"

#define ECUABFEE_WRITE_LAYER ECUABFEE_LAYER_ONE_INDEX

#ifndef ECUABFEE_LOCAL
# define ECUABFEE_LOCAL static
#endif

typedef enum{
  ECUABFEE_WR_STATE_UNINIT = 0,
  ECUABFEE_WR_STATE_IDLE,
  ECUABFEE_WR_STATE_WAIT_FOR_INSTANCEFINDER,
  ECUABFEE_WR_STATE_ERASE_DATASET,
  ECUABFEE_WR_STATE_WRITE_INSTANCE
}EcuabFee_Wr_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_Wr_StateType StateMachine;
  boolean SuspendWrites;
}EcuabFee_Wr_ComponentParameterType;

#define ECUABFEE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Wr_ComponentParameterType, ECUABFEE_APPL_DATA) EcuabFee_Wr_ComponentParameter =
    {MEMIF_JOB_FAILED, MEMIF_UNINIT, ECUABFEE_WR_STATE_UNINIT, FALSE};

#define ECUABFEE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Ih_InstanceType, ECUABFEE_APPL_DATA) EcuabFee_Wr_Instance;

ECUABFEE_LOCAL VAR(EcuabFee_UserJobParameterType, ECUABFEE_APPL_DATA) EcuabFee_Wr_UserJobParameter;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_InitiateDatasetErase(EcuabFee_If_ResultType InstanceFinderResult);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_InitiateWriteInstance(void);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessInstanceFinderState(void);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessDatasetEraseState(void);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessStateMachine(void);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_FinishJob(MemIf_JobResultType JobResult);

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_InitiateDatasetErase(EcuabFee_If_ResultType InstanceFinderResult){
  if(EcuabFee_De_StartJob(InstanceFinderResult) == E_OK){
    EcuabFee_Wr_ComponentParameter.StateMachine = ECUABFEE_WR_STATE_ERASE_DATASET;
  }
  else{
    EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_InitiateWriteInstance(void){
  if(EcuabFee_WrInst_StartJob(EcuabFee_Wr_Instance, EcuabFee_Wr_UserJobParameter) == E_OK){
    EcuabFee_Wr_ComponentParameter.StateMachine = ECUABFEE_WR_STATE_WRITE_INSTANCE;
  }
  else{
    EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessInstanceFinderState(void){
  EcuabFee_If_ResultType InstanceFinderResult = EcuabFee_If_GetJobResult();

  switch(InstanceFinderResult){
  case INSTANCE_FINDER_OK:
  {
    boolean IsLastInstance = EcuabFee_Dh_IsLastInstance(&EcuabFee_Wr_Instance) == TRUE;

    if((EcuabFee_Dh_IsFirstInstanceAndBlank(&EcuabFee_Wr_Instance) == TRUE) || (IsLastInstance == TRUE)){
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

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessDatasetEraseState(void){
  if(EcuabFee_De_GetJobResult() == MEMIF_JOB_OK){
    EcuabFee_Ih_CreateInstance(&EcuabFee_Wr_Instance, EcuabFee_Dh_GetFirstInstanceAddress(), EcuabFee_Dh_GetDataLength());
    EcuabFee_Wr_InitiateWriteInstance();
  }
  else{
    EcuabFee_Wr_FinishJob(EcuabFee_De_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_ProcessStateMachine(void){
  switch(EcuabFee_Wr_ComponentParameter.StateMachine){
    case ECUABFEE_WR_STATE_IDLE:

      if(EcuabFee_If_StartJob(&EcuabFee_Wr_Instance) == E_OK)
      {
        EcuabFee_Wr_ComponentParameter.StateMachine = ECUABFEE_WR_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else
      {
        EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
      }

      break;

    case ECUABFEE_WR_STATE_WAIT_FOR_INSTANCEFINDER:

      EcuabFee_Wr_ProcessInstanceFinderState();

      break;

    case ECUABFEE_WR_STATE_ERASE_DATASET:

      EcuabFee_Wr_ProcessDatasetEraseState();

      break;

    case ECUABFEE_WR_STATE_WRITE_INSTANCE:

      EcuabFee_Wr_FinishJob(EcuabFee_WrInst_GetJobResult());

      break;

    default:
      EcuabFee_Wr_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

ECUABFEE_LOCAL FUNC (void, ECUABFEE_PRIVATE_CODE) EcuabFee_Wr_FinishJob(MemIf_JobResultType JobResult){
  EcuabFee_Tm_RemoveTask(EcuabFee_Wr_Execute, EcuabFee_Wr_Cancel, ECUABFEE_WRITE_LAYER);

  EcuabFee_Wr_ComponentParameter.JobResult = JobResult;
  EcuabFee_Wr_ComponentParameter.StateMachine = ECUABFEE_WR_STATE_IDLE;
  EcuabFee_Wr_ComponentParameter.Status = MEMIF_IDLE;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Init(void){
  EcuabFee_Wr_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Wr_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_Ih_InitInstance(&EcuabFee_Wr_Instance);

  EcuabFee_Wr_ComponentParameter.StateMachine = ECUABFEE_WR_STATE_IDLE;
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_GetStatus(void){
  return EcuabFee_Wr_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_GetJobResult(void){
  return EcuabFee_Wr_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter){
  Std_ReturnType retVal;

  if(EcuabFee_Wr_ComponentParameter.StateMachine == ECUABFEE_WR_STATE_IDLE){
    retVal = EcuabFee_Tm_AddTask(EcuabFee_Wr_Execute, EcuabFee_Wr_Cancel, ECUABFEE_WRITE_LAYER);

    if(retVal == E_OK){
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

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Execute(void){
  if((EcuabFee_Wr_ComponentParameter.Status == MEMIF_BUSY) &&
      (EcuabFee_Wr_ComponentParameter.SuspendWrites == FALSE)){
      EcuabFee_Wr_ProcessStateMachine();
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_Cancel(void){
  EcuabFee_Wr_FinishJob(MEMIF_JOB_CANCELED);
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_SuspendWrites(void){
  EcuabFee_Wr_ComponentParameter.SuspendWrites = TRUE;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Wr_ResumeWrites(void){
  EcuabFee_Wr_ComponentParameter.SuspendWrites = FALSE;
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

