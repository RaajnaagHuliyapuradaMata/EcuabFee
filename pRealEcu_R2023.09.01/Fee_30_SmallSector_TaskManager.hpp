#ifndef FEE_30_SMALLSECTOR_TASKMANAGER_H
#define FEE_30_SMALLSECTOR_TASKMANAGER_H

#include "Fee_30_SmallSector.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"
typedef P2FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE, Fee_30_SmallSector_Tm_ExecuteServicePointerType)(void);
typedef P2FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE, Fee_30_SmallSector_Tm_CancelServicePointerType)(void);
#define FEE_30_SMALLSECTOR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.hpp"

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_Init (void);

FUNC (Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_AddTask (Fee_30_SmallSector_Tm_ExecuteServicePointerType ExecuteService
   ,       Fee_30_SmallSector_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_RemoveTask (Fee_30_SmallSector_Tm_ExecuteServicePointerType ExecuteService
   ,       Fee_30_SmallSector_Tm_CancelServicePointerType CancelService, uint8 Layer);

FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_ExecuteTask (void);
FUNC (void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_Tm_CancelTasks (void);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

