

#define EcuabFee_IMPLEMENTATION_SOURCE

#include "EcuabFee_TaskManager.hpp"

#define EcuabFee_NUMBER_OF_LAYERS        (0x03u)

#ifndef EcuabFee_LOCAL
#define EcuabFee_LOCAL static
#endif

typedef P2FUNC(void, EcuabFee_PRIVATE_CODE, EcuabFee_Tm_FctPtrType)(void);

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

EcuabFee_LOCAL VAR(EcuabFee_Tm_FctPtrType, EcuabFee_APPL_DATA) EcuabFee_Tm_ExecuteServiceStack[EcuabFee_NUMBER_OF_LAYERS];

EcuabFee_LOCAL VAR(EcuabFee_Tm_FctPtrType, EcuabFee_APPL_DATA) EcuabFee_Tm_CancelServiceStack[EcuabFee_NUMBER_OF_LAYERS];

#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"

EcuabFee_LOCAL FUNC (EcuabFee_Tm_FctPtrType, EcuabFee_PRIVATE_CODE) EcuabFee_Tm_GetActiveTask (void);

EcuabFee_LOCAL FUNC (EcuabFee_Tm_FctPtrType, EcuabFee_PRIVATE_CODE) EcuabFee_Tm_GetActiveTask (void){
  sint8 LoopCounter;
  EcuabFee_Tm_FctPtrType returnValue = NULL_PTR;

  for(LoopCounter = EcuabFee_NUMBER_OF_LAYERS - 1; LoopCounter >= 0; LoopCounter--)
  {

   if(EcuabFee_Tm_ExecuteServiceStack[LoopCounter] != NULL_PTR)
   {
      returnValue = EcuabFee_Tm_ExecuteServiceStack[LoopCounter];
      break;
   }
  }
  return returnValue;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_Init(void){
  uint8 LoopCounter;
  for(LoopCounter = 0; LoopCounter < EcuabFee_NUMBER_OF_LAYERS; LoopCounter++)
  {
    EcuabFee_Tm_ExecuteServiceStack[LoopCounter] = NULL_PTR;
    EcuabFee_Tm_CancelServiceStack[LoopCounter] = NULL_PTR;
  }
}

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_AddTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer)
{
  Std_ReturnType retVal;

  if((EcuabFee_Tm_ExecuteServiceStack[Layer] == NULL_PTR) && (EcuabFee_Tm_CancelServiceStack[Layer] == NULL_PTR))
  {
    EcuabFee_Tm_ExecuteServiceStack[Layer] = ExecuteService;
    EcuabFee_Tm_CancelServiceStack[Layer] = CancelService;
    retVal = E_OK;
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_RemoveTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer)
{

  if((EcuabFee_Tm_ExecuteServiceStack[Layer] == ExecuteService) && (EcuabFee_Tm_CancelServiceStack[Layer] == CancelService))
  {
    EcuabFee_Tm_ExecuteServiceStack[Layer] = NULL_PTR;
    EcuabFee_Tm_CancelServiceStack[Layer] = NULL_PTR;
  }
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_ExecuteTask (void){

  EcuabFee_Tm_FctPtrType currentExecuteService = EcuabFee_Tm_GetActiveTask();

  if(currentExecuteService != NULL_PTR)
  {
   currentExecuteService();
  }
}

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_CancelTasks (void){
  sint8 LoopCounter;

  for(LoopCounter = (EcuabFee_NUMBER_OF_LAYERS - 1); LoopCounter >= 0; LoopCounter--)
  {
   if(EcuabFee_Tm_CancelServiceStack[LoopCounter] != NULL_PTR)
   {
      EcuabFee_Tm_CancelServiceStack[LoopCounter]();
   }
  }
}

#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

