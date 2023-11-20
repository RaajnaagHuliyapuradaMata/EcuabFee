#include "Std_Types.hpp"

#define FEE_30_SMALLSECTOR_IMPLEMENTATION_SOURCE

#include "Fee_30_SmallSector_TaskManager.hpp"

#define FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS        (0x03u)

#ifndef FEE_30_SMALLSECTOR_LOCAL
#define FEE_30_SMALLSECTOR_LOCAL static
#endif

typedef P2FUNC(void, FEE_30_SMALLSECTOR_PRIVATE_CODE, Fee_30_SmallSector_Tm_FctPtrType)(void);

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Tm_FctPtrType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Tm_ExecuteServiceStack[FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS];

FEE_30_SMALLSECTOR_LOCAL VAR(Fee_30_SmallSector_Tm_FctPtrType, FEE_30_SMALLSECTOR_APPL_DATA) Fee_30_SmallSector_Tm_CancelServiceStack[FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS];

#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"

FEE_30_SMALLSECTOR_LOCAL FUNC (Fee_30_SmallSector_Tm_FctPtrType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Tm_GetActiveTask (void);

FEE_30_SMALLSECTOR_LOCAL FUNC (Fee_30_SmallSector_Tm_FctPtrType, FEE_30_SMALLSECTOR_PRIVATE_CODE) Fee_30_SmallSector_Tm_GetActiveTask (void){
  sint8 LoopCounter;
  Fee_30_SmallSector_Tm_FctPtrType returnValue = NULL_PTR;

  for(LoopCounter = FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS - 1; LoopCounter >= 0; LoopCounter--)
  {

   if(Fee_30_SmallSector_Tm_ExecuteServiceStack[LoopCounter] != NULL_PTR)
   {
      returnValue = Fee_30_SmallSector_Tm_ExecuteServiceStack[LoopCounter];
      break;
   }
  }
  return returnValue;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_Init(void){
  uint8 LoopCounter;
  for(LoopCounter = 0; LoopCounter < FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS; LoopCounter++)
  {
    Fee_30_SmallSector_Tm_ExecuteServiceStack[LoopCounter] = NULL_PTR;
    Fee_30_SmallSector_Tm_CancelServiceStack[LoopCounter] = NULL_PTR;
  }
}

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_AddTask (Fee_30_SmallSector_Tm_ExecuteServicePointerType ExecuteService
   ,     Fee_30_SmallSector_Tm_CancelServicePointerType CancelService, uint8 Layer)
{
  Std_ReturnType retVal;

  if((Fee_30_SmallSector_Tm_ExecuteServiceStack[Layer] == NULL_PTR) && (Fee_30_SmallSector_Tm_CancelServiceStack[Layer] == NULL_PTR))
  {
    Fee_30_SmallSector_Tm_ExecuteServiceStack[Layer] = ExecuteService;
    Fee_30_SmallSector_Tm_CancelServiceStack[Layer] = CancelService;
    retVal = E_OK;
  }
  else{
    retVal = E_NOT_OK;
  }
  return retVal;
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_RemoveTask (Fee_30_SmallSector_Tm_ExecuteServicePointerType ExecuteService
   ,     Fee_30_SmallSector_Tm_CancelServicePointerType CancelService, uint8 Layer)
{

  if((Fee_30_SmallSector_Tm_ExecuteServiceStack[Layer] == ExecuteService) && (Fee_30_SmallSector_Tm_CancelServiceStack[Layer] == CancelService))
  {
    Fee_30_SmallSector_Tm_ExecuteServiceStack[Layer] = NULL_PTR;
    Fee_30_SmallSector_Tm_CancelServiceStack[Layer] = NULL_PTR;
  }
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_ExecuteTask (void){

  Fee_30_SmallSector_Tm_FctPtrType currentExecuteService = Fee_30_SmallSector_Tm_GetActiveTask();

  if(currentExecuteService != NULL_PTR)
  {
   currentExecuteService();
  }
}

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_CancelTasks (void){
  sint8 LoopCounter;

  for(LoopCounter = (FEE_30_SMALLSECTOR_NUMBER_OF_LAYERS - 1); LoopCounter >= 0; LoopCounter--)
  {
   if(Fee_30_SmallSector_Tm_CancelServiceStack[LoopCounter] != NULL_PTR)
   {
      Fee_30_SmallSector_Tm_CancelServiceStack[LoopCounter]();
   }
  }
}

#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

