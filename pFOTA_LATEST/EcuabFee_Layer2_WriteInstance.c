

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_Layer2_WriteInstance.hpp"

#include "EcuabFee_TaskManager.hpp"
#include "EcuabFee_FlsCoordinator.hpp"

#include "EcuabFee_DatasetHandler.hpp"
#include "EcuabFee_BlockHandler.hpp"
#include "EcuabFee_PartitionHandler.hpp"

#define ECUABFEE_WRITE_INSTANCE_LAYER ECUABFEE_LAYER_TWO_INDEX

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

typedef enum{
  ECUABFEE_WRINST_STATE_UNINIT = 0
   ,  ECUABFEE_WRINST_STATE_IDLE
   ,  ECUABFEE_WRINST_STATE_WRITE_START_PATTERN
   ,  ECUABFEE_WRINST_STATE_VERIFY_START_PATTERN
   ,  ECUABFEE_WRINST_STATE_WRITE_MGMT_PAGE
   ,  ECUABFEE_WRINST_STATE_VERIFY_MGMT_PAGE
   ,  ECUABFEE_WRINST_STATE_WRITE_PAYLOAD
   ,  ECUABFEE_WRINST_STATE_VERIFY_PAYLOAD
   ,  ECUABFEE_WRINST_STATE_WRITE_LAST_PAGE
   ,  ECUABFEE_WRINST_STATE_VERIFY_LAST_PAGE
   ,  ECUABFEE_WRINST_STATE_WRITE_END_PATTERN
   ,  ECUABFEE_WRINST_STATE_VERIFY_END_PATTERN
}EcuabFee_WrInst_StateType;

typedef struct{
  MemIf_JobResultType JobResult;
  MemIf_StatusType Status;
  EcuabFee_WrInst_StateType StateMachine;
}EcuabFee_WrInst_ComponentParameterType;

#define ECUABFEE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_WrInst_ComponentParameterType, ECUABFEE_APPL_DATA) EcuabFee_WrInst_ComponentParameter =
  { MEMIF_JOB_FAILED, MEMIF_UNINIT, ECUABFEE_WRINST_STATE_UNINIT };

#define ECUABFEE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Ih_CompareParameterType, ECUABFEE_APPL_DATA) EcuabFee_WrInst_CompareParameter;

ECUABFEE_LOCAL VAR(EcuabFee_Ih_InstanceType, ECUABFEE_APPL_DATA) EcuabFee_WrInst_Instance;

ECUABFEE_LOCAL VAR(EcuabFee_UserJobParameterType, ECUABFEE_APPL_DATA) EcuabFee_WrInst_UserJobParameter;

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_FinishJob(MemIf_JobResultType JobResult);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_PrepareInstanceForWriting(EcuabFee_Ih_InstanceVarPointerType Instance);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WrInst_StateType nextState);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_StartManagementPageWrite(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_InitiateNextWriteState(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_WriteLastPageOrEndPattern(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_StartEndPatternWrite(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateIdle(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteStartPattern(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteManagementPage(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWritePayload(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteLastPage(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteEndPattern(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateMachine(void);

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_FinishJob(MemIf_JobResultType JobResult){
  EcuabFee_Tm_RemoveTask(EcuabFee_WrInst_Execute, EcuabFee_WrInst_Cancel, ECUABFEE_WRITE_INSTANCE_LAYER);

  EcuabFee_WrInst_ComponentParameter.JobResult = JobResult;
  EcuabFee_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_IDLE;
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_PrepareInstanceForWriting(EcuabFee_Ih_InstanceVarPointerType Instance){
  if(EcuabFee_WrInst_UserJobParameter.JobType == ECUABFEE_WRITE_JOB){
      EcuabFee_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_OK);
  }
  else{
    EcuabFee_Ih_SetInstanceStatus(Instance, INSTANCE_STATUS_INVALID);
  }

  EcuabFee_Ih_SetManagementValuesAccordingToStatus(Instance);
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_InitiateVerifyState(EcuabFee_WrInst_StateType nextState){
  if(EcuabFee_Ih_VerifyData(EcuabFee_WrInst_CompareParameter) == E_OK){
    EcuabFee_WrInst_ComponentParameter.StateMachine = nextState;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_StartManagementPageWrite(void){
  if(EcuabFee_Ih_WriteManagementPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK){
    EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_MGMT_PAGE;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_InitiateNextWriteState(void){
  uint16 PageSize = EcuabFee_Ph_GetPartitionWriteAlignment();
  uint16 MgmtSize = (uint16) ECUABFEE_MANAGEMENT_SIZE;
  uint16 PayloadLength = EcuabFee_Ih_GetInstanceDataLength(&EcuabFee_WrInst_Instance);

  if((PayloadLength <= (PageSize - MgmtSize)) || (EcuabFee_WrInst_UserJobParameter.JobType != ECUABFEE_WRITE_JOB)){
    EcuabFee_WrInst_StartEndPatternWrite();
  }

  else if(PayloadLength > ((2u * PageSize) - MgmtSize)){
    if(EcuabFee_Ih_WritePayload(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK){
      EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_PAYLOAD;
    }
    else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }

  else{
    if(EcuabFee_Ih_WriteLastPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK){
      EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_LAST_PAGE;
    }
    else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_WriteLastPageOrEndPattern(void){
  if(EcuabFee_Ih_IsLastPageNecessary(&EcuabFee_WrInst_Instance) == TRUE){
    if(EcuabFee_Ih_WriteLastPage(&EcuabFee_WrInst_Instance, EcuabFee_WrInst_UserJobParameter, &EcuabFee_WrInst_CompareParameter) == E_OK){
      EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_LAST_PAGE;
    }
    else{
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
    }
  }
  else{
    EcuabFee_WrInst_StartEndPatternWrite();
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_StartEndPatternWrite(void){
  if(EcuabFee_Ih_WriteEndPattern(&EcuabFee_WrInst_Instance, &EcuabFee_WrInst_CompareParameter) == E_OK){
    EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_END_PATTERN;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateIdle(void){
  EcuabFee_WrInst_PrepareInstanceForWriting(&EcuabFee_WrInst_Instance);

  if(EcuabFee_Ih_WriteStartPattern(&EcuabFee_WrInst_Instance, &EcuabFee_WrInst_CompareParameter) == E_OK){
    EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_WRITE_START_PATTERN;
  }
  else{
    EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteStartPattern(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){
    if(EcuabFee_Bh_HasVerificationEnabled() == TRUE){
      EcuabFee_WrInst_InitiateVerifyState(ECUABFEE_WRINST_STATE_VERIFY_START_PATTERN);
    }
    else{
      EcuabFee_WrInst_StartManagementPageWrite();
    }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteManagementPage(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){
    if(EcuabFee_Bh_HasVerificationEnabled() == TRUE){
      EcuabFee_WrInst_InitiateVerifyState(ECUABFEE_WRINST_STATE_VERIFY_MGMT_PAGE);
    }
    else{
      EcuabFee_WrInst_InitiateNextWriteState();
    }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWritePayload(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){
    if(EcuabFee_Bh_HasVerificationEnabled() == TRUE){
      EcuabFee_WrInst_InitiateVerifyState(ECUABFEE_WRINST_STATE_VERIFY_PAYLOAD);
    }
    else{
      EcuabFee_WrInst_WriteLastPageOrEndPattern();
    }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteLastPage(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){
    if(EcuabFee_Bh_HasVerificationEnabled() == TRUE){
      EcuabFee_WrInst_InitiateVerifyState(ECUABFEE_WRINST_STATE_VERIFY_LAST_PAGE);
    }
    else{
      EcuabFee_WrInst_StartEndPatternWrite();
    }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateWriteEndPattern(void){
  if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK){
    if(EcuabFee_Bh_HasVerificationEnabled() == TRUE){
      EcuabFee_WrInst_InitiateVerifyState(ECUABFEE_WRINST_STATE_VERIFY_END_PATTERN);
    }
    else{
      EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
    }
  }
  else{
    EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
  }
}

ECUABFEE_LOCAL FUNC(void, ECUABFEE_PRIVATE_CODE) EcuabFee_WrInst_ProcessStateMachine(void){
  switch(EcuabFee_WrInst_ComponentParameter.StateMachine){
    case ECUABFEE_WRINST_STATE_IDLE:
      EcuabFee_WrInst_ProcessStateIdle();
      break;

    case ECUABFEE_WRINST_STATE_WRITE_START_PATTERN:
      EcuabFee_WrInst_ProcessStateWriteStartPattern();
      break;

    case ECUABFEE_WRINST_STATE_VERIFY_START_PATTERN:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_StartManagementPageWrite();
      }
      else
      {
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

    case ECUABFEE_WRINST_STATE_WRITE_MGMT_PAGE:
      EcuabFee_WrInst_ProcessStateWriteManagementPage();
      break;

    case ECUABFEE_WRINST_STATE_VERIFY_MGMT_PAGE:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_InitiateNextWriteState();
      }
      else
      {
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

    case ECUABFEE_WRINST_STATE_WRITE_PAYLOAD:
      EcuabFee_WrInst_ProcessStateWritePayload();
      break;

    case ECUABFEE_WRINST_STATE_VERIFY_PAYLOAD:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_WriteLastPageOrEndPattern();
      }
      else
      {
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

    case ECUABFEE_WRINST_STATE_WRITE_LAST_PAGE:
      EcuabFee_WrInst_ProcessStateWriteLastPage();
      break;

    case ECUABFEE_WRINST_STATE_VERIFY_LAST_PAGE:
      if(EcuabFee_Fls_GetJobResult() == MEMIF_JOB_OK)
      {
        EcuabFee_WrInst_StartEndPatternWrite();
      }
      else
      {
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      }
      break;

    case ECUABFEE_WRINST_STATE_WRITE_END_PATTERN:
      EcuabFee_WrInst_ProcessStateWriteEndPattern();
      break;

    case ECUABFEE_WRINST_STATE_VERIFY_END_PATTERN:
        EcuabFee_WrInst_FinishJob(EcuabFee_Fls_GetJobResult());
      break;

    default:
      EcuabFee_WrInst_FinishJob(MEMIF_JOB_FAILED);
      break;
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Init(void){
  EcuabFee_WrInst_ComponentParameter.Status = MEMIF_IDLE;
  EcuabFee_WrInst_ComponentParameter.JobResult = MEMIF_JOB_OK;

  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareAddress = 0x0000U;
  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareBufferPtr = NULL_PTR;
  EcuabFee_WrInst_CompareParameter.EcuabFee_Ih_CompareLength = 0x0000U;

  EcuabFee_Ih_InitInstance(&EcuabFee_WrInst_Instance);

  EcuabFee_WrInst_ComponentParameter.StateMachine = ECUABFEE_WRINST_STATE_IDLE;
}

FUNC(MemIf_StatusType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_GetStatus(void){
  return EcuabFee_WrInst_ComponentParameter.Status;
}

FUNC(MemIf_JobResultType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_GetJobResult(void){
  return EcuabFee_WrInst_ComponentParameter.JobResult;
}

FUNC(Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_StartJob(EcuabFee_Ih_InstanceType Instance
   ,       EcuabFee_UserJobParameterType UserJobParameter){
  Std_ReturnType retVal;

  if(EcuabFee_WrInst_ComponentParameter.StateMachine == ECUABFEE_WRINST_STATE_IDLE){
    retVal = EcuabFee_Tm_AddTask(EcuabFee_WrInst_Execute, EcuabFee_WrInst_Cancel, ECUABFEE_WRITE_INSTANCE_LAYER);

    if(retVal == E_OK){
      EcuabFee_WrInst_ComponentParameter.Status = MEMIF_BUSY;
      EcuabFee_WrInst_ComponentParameter.JobResult = MEMIF_JOB_PENDING;

      EcuabFee_WrInst_Instance = Instance;
      EcuabFee_WrInst_UserJobParameter = UserJobParameter;
    }
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Execute(void){
  if(EcuabFee_WrInst_ComponentParameter.Status == MEMIF_BUSY){
    EcuabFee_WrInst_ProcessStateMachine();
  }
}

FUNC(void, ECUABFEE_PUBLIC_CODE) EcuabFee_WrInst_Cancel(void){
  EcuabFee_WrInst_FinishJob(MEMIF_JOB_CANCELED);
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

