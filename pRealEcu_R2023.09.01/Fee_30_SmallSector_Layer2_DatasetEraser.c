

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_Layer2_DatasetEraser.hpp"

#include "Fee_30_SmallSector_TaskManager.hpp"
#include "Fee_30_SmallSector_FlsCoordinator.hpp"
#include "Fee_30_SmallSector_DatasetHandler.hpp"

#define FEE_30_SMALLSECTOR_DATASET_ERASER_LAYER FEE_30_SMALLSECTOR_LAYER_TWO_INDEX

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef enum{
  FEE_30_SMALLSECTOR_DE_STATE_UNINIT = 0
   ,  FEE_30_SMALLSECTOR_DE_STATE_IDLE
   ,  FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1
   ,  FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_ERASE_SECTOR
   ,  FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2
}Fee_30_SmallSector_De_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  Fee_30_SmallSector_De_StateType StateMachine;
}Fee_30_SmallSector_De_ComponentParameterType;

#define FEE_30_SMALLSECTOR_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_De_ComponentParameterType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_De_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, FEE_30_SMALLSECTOR_DE_STATE_UNINIT };

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_If_ResultType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_De_InstanceFinderResult;
FEE_30_SMALLSECTOR_LOCAL VAR(sint16, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_De_SectorIndex;

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_FinishJob(MemIf_JobResultType JobResult);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_EraseLastSector(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateIdle(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateWriteErasePattern1(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateEraseSector(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateMachine(void);

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_FinishJob(MemIf_JobResultType JobResult)
{

  Fee_30_SmallSector_Tm_RemoveTask(Fee_30_SmallSector_De_Execute, Fee_30_SmallSector_De_Cancel, FEE_30_SMALLSECTOR_DATASET_ERASER_LAYER);

  Fee_30_SmallSector_De_ComponentParameter.JobResult = JobResult;
  Fee_30_SmallSector_De_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_De_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_DE_STATE_IDLE;
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_EraseLastSector(void){

  if(Fee_30_SmallSector_Dh_EraseSector((uint16) Fee_30_SmallSector_De_SectorIndex) == E_OK)
  {
    Fee_30_SmallSector_De_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_ERASE_SECTOR;
  }
  else{
    Fee_30_SmallSector_De_FinishJob(MEMIF_JOB_FAILED);
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateIdle(void){

  if((Fee_30_SmallSector_De_InstanceFinderResult == INSTANCE_FINDER_OK) ||
      (Fee_30_SmallSector_De_InstanceFinderResult == INSTANCE_FINDER_EP2_INCORRECT)
     )
  {

   if(Fee_30_SmallSector_Dh_WriteErasePattern(DATASET_ERASE_PATTERN_1) == E_OK)
   {
      Fee_30_SmallSector_De_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1;
   }
   else{
      Fee_30_SmallSector_De_FinishJob(MEMIF_JOB_FAILED);
   }
  }

  else{

    Fee_30_SmallSector_De_EraseLastSector();
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateWriteErasePattern1(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

    Fee_30_SmallSector_De_EraseLastSector();
  }
  else{
    Fee_30_SmallSector_De_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateEraseSector(void){

  if(Fee_30_SmallSector_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

    Fee_30_SmallSector_De_SectorIndex--;

   if(Fee_30_SmallSector_De_SectorIndex >= 0)
   {

      Fee_30_SmallSector_De_EraseLastSector();
   }
   else{

      if(Fee_30_SmallSector_Dh_WriteErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        Fee_30_SmallSector_De_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2;
      }
      else{
        Fee_30_SmallSector_De_FinishJob(MEMIF_JOB_FAILED);
      }
   }
  }
  else{
    Fee_30_SmallSector_De_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
  }
}

FEE_30_SMALLSECTOR_LOCAL FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_De_ProcessStateMachine(void){
  switch(Fee_30_SmallSector_De_ComponentParameter.StateMachine)
  {

   case FEE_30_SMALLSECTOR_DE_STATE_IDLE:
      Fee_30_SmallSector_De_ProcessStateIdle();
      break;

   case FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1:
      Fee_30_SmallSector_De_ProcessStateWriteErasePattern1();
      break;

   case FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_ERASE_SECTOR:
      Fee_30_SmallSector_De_ProcessStateEraseSector();
      break;

   case FEE_30_SMALLSECTOR_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2:
      Fee_30_SmallSector_De_FinishJob(Fee_30_SmallSector_Fls_GetJobResult());
      break;

    default:
      Fee_30_SmallSector_De_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Init(void){

  Fee_30_SmallSector_De_ComponentParameter.Status = MEMIF_IDLE;
  Fee_30_SmallSector_De_ComponentParameter.JobResult = MEMIF_JOB_OK;

  Fee_30_SmallSector_De_InstanceFinderResult = INSTANCE_FINDER_OK;
  Fee_30_SmallSector_De_SectorIndex = 0;

  Fee_30_SmallSector_De_ComponentParameter.StateMachine = FEE_30_SMALLSECTOR_DE_STATE_IDLE;
}

FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_GetStatus(void){
  return Fee_30_SmallSector_De_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_GetJobResult(void){
  return Fee_30_SmallSector_De_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_StartJob(Fee_30_SmallSector_If_ResultType InstanceFinderResult)
{
  Std_ReturnType retVal;

  if(Fee_30_SmallSector_De_ComponentParameter.StateMachine == FEE_30_SMALLSECTOR_DE_STATE_IDLE)
  {

    retVal = Fee_30_SmallSector_Tm_AddTask(Fee_30_SmallSector_De_Execute, Fee_30_SmallSector_De_Cancel, FEE_30_SMALLSECTOR_DATASET_ERASER_LAYER);

   if(retVal == E_OK)
   {
      Fee_30_SmallSector_De_ComponentParameter.Status = MEMIF_BUSY;
      Fee_30_SmallSector_De_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      Fee_30_SmallSector_De_InstanceFinderResult = InstanceFinderResult;

      Fee_30_SmallSector_De_SectorIndex = Fee_30_SmallSector_Dh_GetLastSectorIndex();
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Execute(void){

  if(Fee_30_SmallSector_De_ComponentParameter.Status == MEMIF_BUSY)
  {
    Fee_30_SmallSector_De_ProcessStateMachine();
  }
}

FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_De_Cancel(void){
  Fee_30_SmallSector_De_FinishJob(MEMIF_JOB_CANCELED);
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

