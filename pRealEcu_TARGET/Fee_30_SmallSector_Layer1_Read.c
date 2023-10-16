#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_Layer1_Read.hpp"
#include "Fee_30_SmallSector_Layer2_InstanceFinder.hpp"
#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"
#include "Fee_30_SmallSector_PartitionHandler.hpp"
#include "Fee_30_SmallSector_InstanceHandler.hpp"

#define FEE_30_SMALLSECTOR_READ_LAYER FEE_30_SMALLSECTOR_LAYER_ONE_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
    FEE_30_SMALLSECTOR_RD_STATE_UNINIT = 0
   ,   FEE_30_SMALLSECTOR_RD_STATE_IDLE
   ,   FEE_30_SMALLSECTOR_RD_STATE_WAIT_FOR_INSTANCEFINDER
   ,   FEE_30_SMALLSECTOR_RD_STATE_READ_DATA
}Fee_30_SmallSector_Rd_StateType;

typedef struct{
    MemIf_JobResultType JobResult;
    MemIf_StatusType Status;
    Fee_30_SmallSector_Rd_StateType StateMachine;
}Fee_30_SmallSector_Rd_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Rd_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Rd_ComponentParameter =
   {MEMIF_JOB_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_RD_STATE_UNINIT};

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Ih_InstanceType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Rd_Instance;

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_UserJobParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Rd_UserJobParameter;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Rd_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Rd_ProcessInstanceFinderState(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_Rd_Execute, Fee_30_SmallSector_Rd_Cancel, FEE_30_SMALLSECTOR_READ_LAYER);

  Fee_30_SmallSector_Rd_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_Rd_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_Rd_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RD_STATE_IDLE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Rd_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_Rd_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_RD_STATE_IDLE:

      if(Fee_30_SmallSector_If_StartJob(&Fee_30_SmallSector_Rd_Instance) == E_OK)
      {
        Fee_30_SmallSector_Rd_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RD_STATE_WAIT_FOR_INSTANCEFINDER;
      }
      else{
        Fee_30_SmallSector_Rd_FinishJob(MEMIF_JOB_FAILED);
      }
      break;

   case FEE_30_SMALLSECTOR_RD_STATE_WAIT_FOR_INSTANCEFINDER:

      Fee_30_SmallSector_Rd_ProcessInstanceFinderState();
      break;

   case FEE_30_SMALLSECTOR_RD_STATE_READ_DATA:

      Fee_30_SmallSector_Rd_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      break;

    default:
      Fee_30_SmallSector_Rd_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Rd_ProcessInstanceFinderState(void){

  if(Fee_30_SmallSector_If_MapResult(Fee_30_SmallSector_If_GetJobResult()) == MEMIF_JOB_OK)
  {

   if(Fee_30_SmallSector_Rd_Instance.InstanceStatus == INSTANCE_STATUS_OK)
   {

      if(Fee_30_SmallSector_Ih_ReadData(&Fee_30_SmallSector_Rd_Instance
   ,     Fee_30_SmallSector_Rd_UserJobParameter.DataBufferPtr
   ,     Fee_30_SmallSector_Rd_UserJobParameter.BlockOffset
   ,     Fee_30_SmallSector_Rd_UserJobParameter.Length) == E_OK)
      {
        Fee_30_SmallSector_Rd_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RD_STATE_READ_DATA;
      }
      else{
        Fee_30_SmallSector_Rd_FinishJob(MEMIF_JOB_FAILED);
      }
   }
   else{

      Fee_30_SmallSector_Rd_FinishJob(Fee_30_SmallSector_Ih_SetNegativeJobResultAccordingToStatus(&Fee_30_SmallSector_Rd_Instance));
   }
  }
  else{

    Fee_30_SmallSector_Rd_FinishJob(Fee_30_SmallSector_If_MapResult(Fee_30_SmallSector_If_GetJobResult()));
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_Init(void){
  Fee_30_SmallSector_Rd_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_Rd_ComponentParameter.JobResult = MEMIF_JOB_OK;

  Fee_30_SmallSector_Ih_InitInstance(&Fee_30_SmallSector_Rd_Instance);

  Fee_30_SmallSector_Rd_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_RD_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_GetStatus(void){
  return Fee_30_SmallSector_Rd_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_GetJobResult(void){
  return Fee_30_SmallSector_Rd_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_StartJob(Fee_30_SmallSector_UserJobParameterType Fee_30_SmallSector_UserJobParameter)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_Rd_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_RD_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_Rd_Execute, Fee_30_SmallSector_Rd_Cancel, FEE_30_SMALLSECTOR_READ_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_Rd_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_Rd_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      Fee_30_SmallSector_Rd_UserJobParameter = Fee_30_SmallSector_UserJobParameter;
   }

  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_Execute(void){

  if(Fee_30_SmallSector_Rd_ComponentParameter.Status == MEMIF_BUSY)
  {
    Fee_30_SmallSector_Rd_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Rd_Cancel(void){
  Fee_30_SmallSector_Rd_FinishJob(MEMIF_JOB_CANCELED);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

