#include "Std_Types.hpp"

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer1_Read.hpp"
#include "EcuabFee_Layer2_InstanceFinder.hpp"
#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_PartitionHandler.hpp"
#include "EcuabFee_InstanceHandler.hpp"

#define ECUABFEE_READ_LAYER ECUABFEE_LAYER_ONE_INDEX

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

typedef enum{
    ECUABFEE_RD_STATE_UNINIT = 0
   ,   ECUABFEE_RD_STATE_IDLE
   ,   ECUABFEE_RD_STATE_WAIT_FOR_INSTANCEFINDER
   ,   ECUABFEE_RD_STATE_READ_DATA
}EcuabFee_Rd_StateType;

typedef struct{
    MemIf_JobResultType JobResult;
    MemIf_StatusType Status;
    EcuabFee_Rd_StateType StateMachine;
}EcuabFee_Rd_ComponentParameterType;

#define ECUABFEE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Rd_ComponentParameterType, ECUABFEE_APPL_DATA) EcuabFee_Rd_ComponentParameter =
   {MEMIF_JOB_FAILED, MEMIF_UNINIT, ECUABFEE_RD_STATE_UNINIT};

#define ECUABFEE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Ih_InstanceType, ECUABFEE_APPL_DATA) EcuabFee_Rd_Instance;

ECUABFEE_LOCAL VAR(EcuabFee_UserJobParameterType, ECUABFEE_APPL_DATA) EcuabFee_Rd_UserJobParameter;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_FinishJob(MemIf_JobResultType JobResult);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Rd_ProcessStateMachine(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Rd_ProcessInstanceFinderState(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_Rd_Execute, EcuabFee_Rd_Cancel, ECUABFEE_READ_LAYER);

  EcuabFee_Rd_ComponentParameter.JobResult = JobResult;
  EcuabFee_Rd_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Rd_ComponentParameter.StateMachine = ECUABFEE_RD_STATE_IDLE;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Rd_ProcessStateMachine(void){
  switch(EcuabFee_Rd_ComponentParameter.StateMachine)
  {

   case ECUABFEE_RD_STATE_IDLE:

      if(EcuabFee_If_StartJob(&EcuabFee_Rd_Instance) == E_OK)
      {
        EcuabFee_Rd_ComponentParameter.StateMachine = ECUABFEE_RD_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else{
        EcuabFee_Rd_FinishJob(MEMIF_JOB_FAILED);
      }
      break;

   case ECUABFEE_RD_STATE_WAIT_FOR_INSTANCEFINDER:

      EcuabFee_Rd_ProcessInstanceFinderState();
      break;

   case ECUABFEE_RD_STATE_READ_DATA:

      EcuabFee_Rd_FinishJob(EcuabFee_Fls_GetJobResult());
      break;

    default:
      EcuabFee_Rd_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_Rd_ProcessInstanceFinderState(void){

  if(EcuabFee_If_MapResult(EcuabFee_If_GetJobResult()) == MEMIF_JOB_OK)
  {

   if(EcuabFee_Rd_Instance.InstanceStatus == INSTANCE_STATUS_OK)
   {

      if(EcuabFee_Ih_ReadData(&EcuabFee_Rd_Instance
   ,     EcuabFee_Rd_UserJobParameter.DataBufferPtr
   ,     EcuabFee_Rd_UserJobParameter.BlockOffset
   ,     EcuabFee_Rd_UserJobParameter.Length) == E_OK)
      {
        EcuabFee_Rd_ComponentParameter.StateMachine = ECUABFEE_RD_STATE_READ_DATA;
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

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Init(void){
  EcuabFee_Rd_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_Rd_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_Ih_InitInstance(&EcuabFee_Rd_Instance);

  EcuabFee_Rd_ComponentParameter.StateMachine = ECUABFEE_RD_STATE_IDLE;
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_GetStatus(void){
  return EcuabFee_Rd_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_GetJobResult(void){
  return EcuabFee_Rd_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_StartJob(EcuabFee_UserJobParameterType EcuabFee_UserJobParameter)
{
  Std_ReturnType retVal;

  if(EcuabFee_Rd_ComponentParameter.StateMachine == ECUABFEE_RD_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_Rd_Execute, EcuabFee_Rd_Cancel, ECUABFEE_READ_LAYER);

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

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Execute(void){

  if(EcuabFee_Rd_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_Rd_ProcessStateMachine();
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_Rd_Cancel(void){
  EcuabFee_Rd_FinishJob(MEMIF_JOB_CANCELED);
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

