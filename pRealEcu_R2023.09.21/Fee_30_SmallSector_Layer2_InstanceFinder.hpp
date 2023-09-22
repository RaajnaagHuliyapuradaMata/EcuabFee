#ifndef FEE_30_SMALLSECTOR_SUB_INSTANCE_FINDER_H
#define FEE_30_SMALLSECTOR_SUB_INSTANCE_FINDER_H

#include "Fee_30_SmallSector.hpp"
#include "Fee_30_SmallSector_InstanceHandler.hpp"

typedef enum{
  INSTANCE_FINDER_OK = 0
   ,  INSTANCE_FINDER_PENDING
   ,  INSTANCE_FINDER_CANCELED
   ,  INSTANCE_FINDER_FAILED
   ,  INSTANCE_FINDER_INCONSISTENT
   ,  INSTANCE_FINDER_INVALID
   ,  INSTANCE_FINDER_EP1_INCORRECT
   ,  INSTANCE_FINDER_EP2_INCORRECT
}Fee_30_SmallSector_If_ResultType;

#define FEE_30_SMALLSECTOR_START_SEC_CODE
#include "MemMap.hpp"
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Init(void);
FUNC(MemIf_StatusType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_GetStatus(void);
FUNC(Fee_30_SmallSector_If_ResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_GetJobResult(void);
FUNC(Std_ReturnType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_StartJob(Fee_30_SmallSector_Ih_InstanceVarPointerType Instance);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Execute(void);
FUNC(void, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_Cancel(void);
FUNC(MemIf_JobResultType, FEE_30_SMALLSECTOR_PUBLIC_CODE) Fee_30_SmallSector_If_MapResult(Fee_30_SmallSector_If_ResultType JobResult);
#define FEE_30_SMALLSECTOR_STOP_SEC_CODE
#include "MemMap.hpp"

#endif

