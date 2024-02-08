#ifndef EcuabFee_TASKMANAGER_H
#define EcuabFee_TASKMANAGER_H

#include "Fee_30_SmallSector.hpp"

#define EcuabFee_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
typedef P2FUNC(void, EcuabFee_PUBLIC_CODE, EcuabFee_Tm_ExecuteServicePointerType)(void);
typedef P2FUNC(void, EcuabFee_PUBLIC_CODE, EcuabFee_Tm_CancelServicePointerType)(void);
#define EcuabFee_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define EcuabFee_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_Init (void);

FUNC (Std_ReturnType, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_AddTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,     EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_RemoveTask (EcuabFee_Tm_ExecuteServicePointerType ExecuteService
   ,     EcuabFee_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_ExecuteTask (void);
FUNC (void, EcuabFee_PUBLIC_CODE) EcuabFee_Tm_CancelTasks (void);
#define EcuabFee_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

