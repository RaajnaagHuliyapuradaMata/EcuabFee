#include "Std_Types.hpp"

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer2_DatasetEraser.hpp"
#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"
#include "EcuabFee_DatasetHandler.hpp"

#define ECUABFEE_DATASET_ERASER_LAYER ECUABFEE_LAYER_TWO_INDEX

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

typedef enum{
  ECUABFEE_DE_STATE_UNINIT = 0
   ,  ECUABFEE_DE_STATE_IDLE
   ,  ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1
   ,  ECUABFEE_DE_STATE_WAITFOR_ERASE_SECTOR
   ,  ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2
}EcuabFee_De_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_De_StateType StateMachine;
}EcuabFee_De_ComponentParameterType;

#define ECUABFEE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_De_ComponentParameterType, ECUABFEE_APPL_DATA) EcuabFee_De_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, ECUABFEE_DE_STATE_UNINIT };

#define ECUABFEE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_If_ResultType, ECUABFEE_APPL_DATA) EcuabFee_De_InstanceFinderResult;
ECUABFEE_LOCAL VAR(sint16, ECUABFEE_APPL_DATA) EcuabFee_De_SectorIndex;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_FinishJob(MemIf_JobResultType JobResult);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_EraseLastSector(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateIdle(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateWriteErasePattern1(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateEraseSector(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateMachine(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_FinishJob(MemIf_JobResultType JobResult)
{

  EcuabFee_Tm_RemoveTask(EcuabFee_De_Execute, EcuabFee_De_Cancel, ECUABFEE_DATASET_ERASER_LAYER);

  EcuabFee_De_ComponentParameter.JobResult = JobResult;
  EcuabFee_De_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_De_ComponentParameter.StateMachine = ECUABFEE_DE_STATE_IDLE;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_EraseLastSector(void){

  if(EcuabFee_Dh_EraseSector((uint16) EcuabFee_De_SectorIndex) == E_OK)
  {
    EcuabFee_De_ComponentParameter.StateMachine = ECUABFEE_DE_STATE_WAITFOR_ERASE_SECTOR;
  }
  else{
    EcuabFee_De_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateIdle(void){

  if((EcuabFee_De_InstanceFinderResult == INSTANCE_FINDER_OK) ||
      (EcuabFee_De_InstanceFinderResult == INSTANCE_FINDER_EP2_INCORRECT)
     )
  {

   if(EcuabFee_Dh_WriteErasePattern(DATASET_ERASE_PATTERN_1) == E_OK)
   {
      EcuabFee_De_ComponentParameter.StateMachine = ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1;
   }
   else{
      EcuabFee_De_FinishJob(MEMIF_JOB_FAILED);
   }
  }

  else{

    EcuabFee_De_EraseLastSector();
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateWriteErasePattern1(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

    EcuabFee_De_EraseLastSector();
  }
  else{
    EcuabFee_De_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateEraseSector(void){

  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
  {

    EcuabFee_De_SectorIndex--;

   if(EcuabFee_De_SectorIndex >= 0)
   {

      EcuabFee_De_EraseLastSector();
   }
   else{

      if(EcuabFee_Dh_WriteErasePattern(DATASET_ERASE_PATTERN_2) == E_OK)
      {
        EcuabFee_De_ComponentParameter.StateMachine = ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2;
      }
      else{
        EcuabFee_De_FinishJob(MEMIF_JOB_FAILED);
      }
   }
  }
  else{
    EcuabFee_De_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_De_ProcessStateMachine(void){
  switch(EcuabFee_De_ComponentParameter.StateMachine)
  {

   case ECUABFEE_DE_STATE_IDLE:
      EcuabFee_De_ProcessStateIdle();
      break;

   case ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN1:
      EcuabFee_De_ProcessStateWriteErasePattern1();
      break;

   case ECUABFEE_DE_STATE_WAITFOR_ERASE_SECTOR:
      EcuabFee_De_ProcessStateEraseSector();
      break;

   case ECUABFEE_DE_STATE_WAITFOR_WRITE_ERASE_PATTERN2:
      EcuabFee_De_FinishJob(EcuabFee_Fls_GetJobResult());
      break;

    default:
      EcuabFee_De_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Init(void){

  EcuabFee_De_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_De_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_De_InstanceFinderResult = INSTANCE_FINDER_OK;
  EcuabFee_De_SectorIndex = 0;

  EcuabFee_De_ComponentParameter.StateMachine = ECUABFEE_DE_STATE_IDLE;
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_GetStatus(void){
  return EcuabFee_De_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_GetJobResult(void){
  return EcuabFee_De_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_De_StartJob(EcuabFee_If_ResultType InstanceFinderResult)
{
  Std_ReturnType retVal;

  if(EcuabFee_De_ComponentParameter.StateMachine == ECUABFEE_DE_STATE_IDLE)
  {

    retVal = EcuabFee_Tm_AddTask(EcuabFee_De_Execute, EcuabFee_De_Cancel, ECUABFEE_DATASET_ERASER_LAYER);

   if(retVal == E_OK)
   {
      EcuabFee_De_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_De_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_De_InstanceFinderResult = InstanceFinderResult;

      EcuabFee_De_SectorIndex = EcuabFee_Dh_GetLastSectorIndex();
   }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Execute(void){

  if(EcuabFee_De_ComponentParameter.Status == MEMIF_BUSY)
  {
    EcuabFee_De_ProcessStateMachine();
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_De_Cancel(void){
  EcuabFee_De_FinishJob(MEMIF_JOB_CANCELED);
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

