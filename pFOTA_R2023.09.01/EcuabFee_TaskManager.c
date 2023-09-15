

#define ECUABFEE_IMPLEMENTATION_SOURCE

#include "EcuabFee_TaskManager.hpp"

#define ECUABFEE_NUMBER_OF_LAYERS        (0x03u)

#ifndef ECUABFEE_LOCAL
#define ECUABFEE_LOCAL static
#endif

typedef P2FUNC(void, ECUABFEE_PRIVATE_CODE, EcuabFee_Tm_FctPtrType)(void);

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

ECUABFEE_LOCAL VAR(EcuabFee_Tm_FctPtrType, ECUABFEE_APPL_DATA) EcuabFee_Tm_ExecuteServiceStack[ECUABFEE_NUMBER_OF_LAYERS];

ECUABFEE_LOCAL VAR(EcuabFee_Tm_FctPtrType, ECUABFEE_APPL_DATA) EcuabFee_Tm_CancelServiceStack[ECUABFEE_NUMBER_OF_LAYERS];

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

ECUABFEE_LOCAL FUNC (EcuabFee_Tm_FctPtrType, ECUABFEE_PRIVATE_CODE) EcuabFee_Tm_GetActiveTask (void);

ECUABFEE_LOCAL FUNC (EcuabFee_Tm_FctPtrType, ECUABFEE_PRIVATE_CODE) EcuabFee_Tm_GetActiveTask (void){
  sint8 LoopCounter;
  EcuabFee_Tm_FctPtrType returnValue = NULL_PTR;

  for(LoopCounter = ECUABFEE_NUMBER_OF_LAYERS - 1; LoopCounter >= 0; LoopCounter--){
    if(EcuabFee_Tm_ExecuteServiceStack[LoopCounter] != NULL_PTR){
      returnValue = EcuabFee_Tm_ExecuteServiceStack[LoopCounter];
      break;
    }
  }
  return returnValue;
}

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_Init(void){
  uint8 LoopCounter;
  for(LoopCounter = 0; LoopCounter < ECUABFEE_NUMBER_OF_LAYERS; LoopCounter++){
    EcuabFee_Tm_ExecuteServiceStack[LoopCounter] = NULL_PTR;
    EcuabFee_Tm_CancelServiceStack[LoopCounter] = NULL_PTR;
  }
}

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_AddTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer){
  Std_ReturnType retVal;

  if((EcuabFee_Tm_ExecuteServiceStack[Layer] == NULL_PTR) && (EcuabFee_Tm_CancelServiceStack[Layer] == NULL_PTR)){
    EcuabFee_Tm_ExecuteServiceStack[Layer] = ExecuteService;
    EcuabFee_Tm_CancelServiceStack[Layer] = CancelService;
    retVal = E_OK;
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_RemoveTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer){
  if((EcuabFee_Tm_ExecuteServiceStack[Layer] == ExecuteService) && (EcuabFee_Tm_CancelServiceStack[Layer] == CancelService)){
    EcuabFee_Tm_ExecuteServiceStack[Layer] = NULL_PTR;
    EcuabFee_Tm_CancelServiceStack[Layer] = NULL_PTR;
  }
}

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_ExecuteTask (void){
  EcuabFee_Tm_FctPtrType currentExecuteService = EcuabFee_Tm_GetActiveTask();

  if(currentExecuteService != NULL_PTR){
    currentExecuteService();
  }
}

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_CancelTasks (void){
  sint8 LoopCounter;

  for(LoopCounter = (ECUABFEE_NUMBER_OF_LAYERS - 1); LoopCounter >= 0; LoopCounter--){
    if(EcuabFee_Tm_CancelServiceStack[LoopCounter] != NULL_PTR){
      EcuabFee_Tm_CancelServiceStack[LoopCounter]();
    }
  }
}

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

