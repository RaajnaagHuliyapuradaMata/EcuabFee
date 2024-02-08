

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer1_Read.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"

#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#include "EcuabFee_PartitionHandler.hpp"
#include "EcuabFee_InstanceHandler.hpp"

#define EcuabFee_READ_LAYER EcuabFee_LAYER_ONE_INDEX

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef enum{
    EcuabFee_RD_STATE_UNINIT = 0
   ,   EcuabFee_RD_STATE_IDLE
   ,   EcuabFee_RD_STATE_WAIT_FOR_INSTANCEFINDER
   ,   EcuabFee_RD_STATE_READ_DATA
}EcuabFee_Rd_StateType;

typedef struct{
    MemIf_JobResultType JobResult;
    MemIf_StatusType Status;
    EcuabFee_Rd_StateType StateMachine;
}EcuabFee_Rd_ComponentParameterType;

#define EcuabFee_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Rd_ComponentParameterType, EcuabFee_APPL_DATA) EcuabFee_Rd_ComponentParameter =
   {MEMIF_JOB_FAILED, MEMIF_UNINIT, EcuabFee_RD_STATE_UNINIT};

#define EcuabFee_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Ih_InstanceType, EcuabFee_APPL_DATA) EcuabFee_Rd_Instance;

EcuabFee_LOCAL VAR(EcuabFee_UserJobParameterType, EcuabFee_APPL_DATA) EcuabFee_Rd_UserJobParameter;

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_FinishJob(MemIf_JobResultType JobResult);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Rd_ProcessStateMachine(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Rd_ProcessInstanceFinderState(void);

EcuabFee_LOCAL FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_Rd_Execute, EcuabFee_Rd_Cancel, EcuabFee_READ_LAYER);

  EcuabFee_Rd_ComponentParameter.JobResult = JobResult;
  EcuabFee_Rd_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Rd_ComponentParameter.StateMachine = EcuabFee_RD_STATE_IDLE;
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Rd_ProcessStateMachine(void){
  switch(EcuabFee_Rd_ComponentParameter.StateMachine)
  {

   case EcuabFee_RD_STATE_IDLE:

      if(EcuabFee_If_StartJob(&EcuabFee_Rd_Instance) == E_OK)
      {
        EcuabFee_Rd_ComponentParameter.StateMachine = EcuabFee_RD_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else{
        EcuabFee_Rd_FinishJob(MEMIF_JOB_FAILED);
      }
      break;

   case EcuabFee_RD_STATE_WAIT_FOR_INSTANCEFINDER:

      EcuabFee_Rd_ProcessInstanceFinderState();
      break;

   case EcuabFee_RD_STATE_READ_DATA:

      EcuabFee_Rd_FinishJob(EcuabFee_Fls_GetJobResult());
      break;

    default:
      EcuabFee_Rd_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

EcuabFee_LOCAL FUNC(void, EcuabFee_PRIVATE_CODE) EcuabFee_Rd_ProcessInstanceFinderState(void){

  if(EcuabFee_If_MapResult(EcuabFee_If_GetJobResult()) == MEMIF_JOB_OK)
  {

   if(EcuabFee_Rd_Instance.InstanceStatus == INSTANCE_STATUS_OK)
   {

      if(EcuabFee_Ih_ReadData(&EcuabFee_Rd_Instance
   ,                     EcuabFee_Rd_UserJobParameter.DataBufferPtr
   ,                     EcuabFee_Rd_UserJobParameter.BlockOffset
   ,                     EcuabFee_Rd_UserJobParameter.Length) == E_OK)
      {
        EcuabFee_Rd_ComponentParameter.StateMachine = EcuabFee_RD_STATE_READ_DATA;
      }
      else{
        EcuabFee_Rd_FinishJob(MEMIF_JOB_FAILED);
      }
   }
   else{

      EcuabFee_Rd_FinishJob(EcuabFee_Ih_SetNegativeJobResultAccordingToStatus(&EcuabFee_Rd_Instance));
   }
  }
  else{

    EcuabFee_Rd_FinishJob(EcuabFee_If_MapResult(EcuabFee_If_GetJobResult()));
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Init(void){
  EcuabFee_Rd_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Rd_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_Ih_InitInstance(&EcuabFee_Rd_Instance);

  EcuabFee_Rd_ComponentParameter.StateMachine = EcuabFee_RD_STATE_IDLE;
}

FUNC(MemIf_StatusType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_GetStatus(void){
  return EcuabFee_Rd_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_GetJobResult(void){
  return EcuabFee_Rd_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter)
{
  Std_ReturnType retVal;

  if(EcuabFee_Rd_ComponentParameter.StateMachine == EcuabFee_RD_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_Rd_Execute, EcuabFee_Rd_Cancel, EcuabFee_READ_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_Rd_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_Rd_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_Rd_UserJobParameter = EcuabFee_UserJobParameter;
   }

  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Execute(void){

  if(EcuabFee_Rd_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_Rd_ProcessStateMachine();
  }
}

FUNC(void, EcuabFee_PUBLIC_CODE) EcuabFee_Rd_Cancel(void){
  EcuabFee_Rd_FinishJob(MEMIF_JOB_CANCELED);
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

