

#ifndef ECUABFEE_TASKMANAGER_H
#define ECUABFEE_TASKMANAGER_H

#include "EcuabFee.hpp"

#define ECUABFEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

typedef P2FUNC(void, ECUABFEE_PUBLIC_CODE, EcuabFee_Tm_ExecuteServicePointerType)(void);
typedef P2FUNC(void, ECUABFEE_PUBLIC_CODE, EcuabFee_Tm_CancelServicePointerType)(void);

#define ECUABFEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define ECUABFEE_START_SEC_CODE
#include "MemMap.hpp"

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_Init (void);

FUNC (Std_ReturnType, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_AddTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_RemoveTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,       EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_ExecuteTask (void);

FUNC (void, ECUABFEE_PUBLIC_CODE) EcuabFee_Tm_CancelTasks (void);

#define ECUABFEE_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

